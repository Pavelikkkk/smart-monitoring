import { request, setToken } from "../../shared/api/client";
import type { AccountType } from "../../shared/api/types";

export async function registerUser(input: {
  username: string;
  email: string;
  password: string;
  accountType: AccountType;
}) {
  return request<{ userId: number }>("/auth/register", {
    method: "POST",
    body: JSON.stringify(input),
  });
}

export async function loginUser(input: { email: string; password: string }) {
  const response = await request<{ token: string }>("/auth/login", {
    method: "POST",
    body: JSON.stringify(input),
  });

  setToken(response.token);

  return response;
}
