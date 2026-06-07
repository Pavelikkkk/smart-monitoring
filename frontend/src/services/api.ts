const API_URL =
  import.meta.env.PROD
    ? "https://potential-centered-talent-correction.trycloudflare.com"
    : "http://localhost:8080";

export async function getStats() {
  const response =
    await fetch(`${API_URL}/api/stats`);

  return response.json();
}

export async function getHealth() {
  const response =
    await fetch(`${API_URL}/api/health`);

  return response.json();
}

export async function getAnomalies() {
  const response =
    await fetch(`${API_URL}/api/anomalies/latest`);

  return response.json();
}

export async function getRooms() {
  const response =
    await fetch(`${API_URL}/api/rooms`);

  return response.json();
}

export async function getPowerHistory() {
  const response =
    await fetch(`${API_URL}/api/power/history`);

  return response.json();
}
export async function getDevices() {
  const response =
    await fetch(`${API_URL}/api/devices`);

  return response.json();
}

export async function getBuildings() {
  const response =
    await fetch(`${API_URL}/api/buildings`);

  return response.json();
}

export async function getTopConsumers() {
  const response =
    await fetch(
      `${API_URL}/api/analytics/top-consumers`
    );

  return response.json();
}

export async function getAnomalyStatistics() {
  const response =
    await fetch(
      `${API_URL}/api/analytics/anomalies-by-type`
    );

  return response.json();
}
export async function getEnergyByRoom() {
  const response =
    await fetch(
      `${API_URL}/api/analytics/energy-by-room`
    );

  return response.json();
}

export async function getSeverityDistribution() {
  const response =
    await fetch(
      `${API_URL}/api/analytics/severity-distribution`
    );

  return response.json();
}