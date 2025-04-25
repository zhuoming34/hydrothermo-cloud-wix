import { fetch } from 'wix-fetch';
import { webMethod, Permissions } from 'wix-web-module';
import { ENVDATA_API_URL } from './constantsBackend';

export const readEnvDataFromCloud = webMethod(
  Permissions.Anyone, // or Permissions.Admin if restricted
  async (table_name, device_id, data_amount) => {
    console.log("✅ Backend webMethod called");

    const queryString = `table_name=${table_name}&device_id=${device_id}&data_amount=${data_amount}`;
    const url = `${ENVDATA_API_URL}?${queryString}`;
    console.log("🌐 Fetching from:", url);

    try {
      const response = await fetch(url, { method: "GET" });

      if (!response.ok) {
        throw new Error(`HTTP Error: ${response.status}`);
      }

      const data = await response.json();

      const myDevice = data.deviceData?.[0].device_id;

      const readings = data.deviceData.map(entry => ({
        timestamp: Number(entry.timestamp),
        temperature: parseFloat(entry.env_data.temperature),
        humidity: parseFloat(entry.env_data.humidity),
      }));

      return { myDevice, readings };

    } catch (err) {
      console.error("❌ Error fetching data:", err);
      return { myDevice: "error", readings: [] };
    }
  }
);
