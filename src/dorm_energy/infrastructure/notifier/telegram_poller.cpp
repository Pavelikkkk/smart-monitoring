#include "dorm_energy/infrastructure/notifier/telegram_poller.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

namespace dorm_energy::notifier
{
    TelegramPoller::TelegramPoller(
        const TelegramConfig &config)
        : config_(config)
    {
        curl_global_init(CURL_GLOBAL_DEFAULT); // перенсти в main

        if (!config_.botToken.empty())
        {
            baseUrl_ =
                "https://api.telegram.org/bot" + config_.botToken;

            std::cout
                << "[TelegramPoller] Initialized\n";
        }
    }

    TelegramPoller::~TelegramPoller()
    {
        stop();
        curl_global_cleanup();
    }

    void TelegramPoller::start()
    {
        std::cout
            << "[TelegramPoller] start()\n";
        if (running_ || baseUrl_.empty())
            return;

        running_ = true;
        try
        {
            pollThread_ = std::thread(&TelegramPoller::pollingLoop, this);
            std::cout << "[TelegramPoller] Polling thread started\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << "[TelegramPoller] Failed to start thread: " << e.what() << std::endl;
            running_ = false;
        }
    }

    void TelegramPoller::stop()
    {
        if (!running_)
            return;
        running_ = false;
        if (pollThread_.joinable())
            pollThread_.join();
    }

    void TelegramPoller::pollingLoop()
    {
        std::cout << "[TelegramPoller] Polling loop started\n";

        while (running_)
        {
            try
            {
                CURL *curl = curl_easy_init();
                if (!curl)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    continue;
                }

                std::string url =
                    "https://api.telegram.org";
                std::string response;

                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(
                    curl,
                    CURLOPT_NOSIGNAL,
                    1L);
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                                 [](char *ptr, size_t size, size_t nmemb, void *data) -> size_t
                                 {
                                     ((std::string *)data)->append(ptr, size * nmemb);
                                     return size * nmemb;
                                 });
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

                std::cout
                    << "[TelegramPoller] before perform\n";

                CURLcode res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                if (res == CURLE_OK && !response.empty())
                {
                    try
                    {
                        auto j = json::parse(response, nullptr, false);
                        if (j.is_discarded() || !j.contains("result"))
                            continue;

                        for (const auto &update : j["result"])
                        {
                            long updateId = update.value("update_id", 0L);
                            if (updateId > lastUpdateId_)
                                lastUpdateId_ = updateId;

                            if (update.contains("callback_query"))
                            {
                                const auto &cb = update["callback_query"];
                                if (cb.contains("data") && cb.contains("message"))
                                {
                                    std::string data = cb.value("data", "");
                                    long messageId = cb["message"].value("message_id", 0L);
                                    long chatId = cb["message"]["chat"].value("id", 0L);

                                    if (!data.empty() && messageId > 0)
                                    {
                                        handleCallbackQuery(data, messageId, chatId);
                                    }
                                }
                            }
                        }
                    }
                    catch (...)
                    {
                    }
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "[TelegramPoller] Error: " << e.what() << std::endl;
            }
            catch (...)
            {
                std::cerr << "[TelegramPoller] Unknown error\n";
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1200));
        }
    }

    bool TelegramPoller::handleCallbackQuery(const std::string &callbackData, long messageId, long chatId)
    {
        if (callbackData.find("ack") != 0)
            return false;

        auto now = std::chrono::system_clock::now();
        auto tt = std::chrono::system_clock::to_time_t(now);
        std::ostringstream timeStr;
        timeStr << std::put_time(std::localtime(&tt), "%H:%M:%S");

        std::string newText = "✅ *ПРИНЯТО В РАБОТУ*\n\nВремя: " + timeStr.str();

        editMessage(chatId, messageId, newText);
        return true;
    }

    bool TelegramPoller::editMessage(long chatId, long messageId, const std::string &newText)
    {
        CURL *curl = curl_easy_init();
        if (!curl)
            return false;

        char *encoded = curl_easy_escape(curl, newText.c_str(), static_cast<int>(newText.length()));
        if (!encoded)
        {
            curl_easy_cleanup(curl);
            return false;
        }

        std::string postData = "chat_id=" + std::to_string(chatId) +
                               "&message_id=" + std::to_string(messageId) +
                               "&text=" + encoded +
                               "&parse_mode=Markdown";

        curl_easy_setopt(curl, CURLOPT_URL, (baseUrl_ + "/editMessageText").c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6L);

        CURLcode res = curl_easy_perform(curl);

        curl_free(encoded);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
            std::cout << "[TelegramPoller] Message edited\n";

        return res == CURLE_OK;
    }

} // namespace dorm_energy::notifier