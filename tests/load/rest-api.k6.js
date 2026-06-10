import http from "k6/http";
import { check, sleep } from "k6";

const baseUrl = __ENV.BASE_URL || "http://host.docker.internal:8080/api";

export const options = {
  stages: [
    { duration: "10s", target: 10 },
    { duration: "30s", target: 10 },
    { duration: "10s", target: 0 },
  ],
  thresholds: {
    http_req_failed: ["rate<0.01"],
    http_req_duration: ["p(95)<500"],
  },
};

export function setup() {
  const suffix = `${Date.now()}-${Math.floor(Math.random() * 100000)}`;
  const email = `k6.load.${suffix}@example.com`;
  const password = "CorrectPass123!";

  const register = http.post(
    `${baseUrl}/auth/register`,
    JSON.stringify({
      username: `k6load${suffix}`,
      email,
      password,
      accountType: "PERSONAL",
    }),
    { headers: { "Content-Type": "application/json" } }
  );

  check(register, {
    "register succeeded": (response) =>
      response.status === 200 && response.json("success") === true,
  });

  const login = http.post(
    `${baseUrl}/auth/login`,
    JSON.stringify({ email, password }),
    { headers: { "Content-Type": "application/json" } }
  );

  check(login, {
    "login succeeded": (response) =>
      response.status === 200 && Boolean(response.json("token")),
  });

  return {
    token: login.json("token"),
  };
}

export default function (data) {
  const params = {
    headers: {
      Authorization: `Bearer ${data.token}`,
    },
  };

  const responses = http.batch([
    ["GET", `${baseUrl}/buildings`, null, params],
    ["GET", `${baseUrl}/rooms`, null, params],
    ["GET", `${baseUrl}/devices`, null, params],
    ["GET", `${baseUrl}/anomalies/latest`, null, params],
    ["GET", `${baseUrl}/stats`, null, params],
    ["GET", `${baseUrl}/analytics/energy-by-room`, null, params],
    ["GET", `${baseUrl}/analytics/top-consumers`, null, params],
    ["GET", `${baseUrl}/analytics/severity-distribution`, null, params],
    ["GET", `${baseUrl}/power/history`, null, params],
  ]);

  check(responses, {
    "all API responses are 200": (items) =>
      items.every((response) => response.status === 200),
  });

  sleep(1);
}
