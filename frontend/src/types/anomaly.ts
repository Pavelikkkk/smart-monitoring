export type Severity = "INFO" | "WARNING" | "CRITICAL";

export interface Anomaly {
  id: number;

  roomId: string;

  anomalyType: string;

  severity: Severity;

  score?: number;

  description: string;

  detectedAt: string;

  acknowledged: boolean;
}
