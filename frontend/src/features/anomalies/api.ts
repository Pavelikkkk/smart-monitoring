import { request } from "../../shared/api/client";
import type { Anomaly } from "../../shared/api/types";

export async function getUserAnomalies() {
  return request<Anomaly[]>("/anomalies/latest");
}
