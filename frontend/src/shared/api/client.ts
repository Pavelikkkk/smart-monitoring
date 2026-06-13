const API_BASE_URL =
  import.meta.env.VITE_API_URL ?? "http://localhost:8080/api";

const TOKEN_KEY = "dorm_energy_token";

type ApiResult<T> = T & {
  success?: boolean;
  error?: string;
};

function getToken() {
  return localStorage.getItem(TOKEN_KEY);
}

export function setToken(token: string) {
  localStorage.setItem(TOKEN_KEY, token);
}

export function clearToken() {
  localStorage.removeItem(TOKEN_KEY);
}

export async function request<T>(
  path: string,
  options: RequestInit = {},
): Promise<T> {
  const headers = new Headers(options.headers);

  headers.set("Content-Type", "application/json");

  const token = getToken();

  if (token) {
    headers.set("Authorization", `Bearer ${token}`);
  }

  const response = await fetch(`${API_BASE_URL}${path}`, {
    ...options,
    headers,
  });

  const data = (await response.json()) as ApiResult<T>;

  if (!response.ok || data.success === false) {
    throw new Error(data.error ?? "Request failed");
  }

  return data as T;
}
