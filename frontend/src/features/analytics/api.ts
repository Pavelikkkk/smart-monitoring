import { request } from "../../shared/api/client";
import type {
  AnomalyStatistic,
  EnergyByRoom,
  PowerPoint,
  SeverityDistribution,
  TopConsumer,
} from "../../shared/api/types";

export async function getPowerHistory() {
  return request<PowerPoint[]>("/power/history");
}

export async function getTopConsumers() {
  return request<TopConsumer[]>("/analytics/top-consumers");
}

export async function getAnomalyStatistics() {
  return request<AnomalyStatistic[]>("/analytics/anomalies-by-type");
}

export async function getSeverityDistribution() {
  return request<SeverityDistribution[]>("/analytics/severity-distribution");
}

export async function getEnergyByRoom() {
  return request<EnergyByRoom[]>("/analytics/energy-by-room");
}
