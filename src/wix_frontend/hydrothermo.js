/* Wix page code for displaying the readings */

// Velo API Reference: https://www.wix.com/velo/reference/api-overview/introduction
// import { readEnvDataFromCloud } from 'backend/readEnvDataWix'; // .jsw version
import { readEnvDataFromCloud } from 'backend/readEnvData.web'; // .web.js version
import { TABLE_NAME, DEVICE_ID, DATA_FETCH_AMOUNT } from 'public/constantsFrontend';

async function loadChartData() {
    const data = await readEnvDataFromCloud(TABLE_NAME, DEVICE_ID, DATA_FETCH_AMOUNT);
    console.log(data);

    const myDevice = data.myDevice;
    if (myDevice == DEVICE_ID) {
        const readings = data.readings;
        const timestamps = readings.map(r => new Date(r.timestamp * 1000).toLocaleString());
        const temperatures = readings.map(r => r.temperature);
        const humidities = readings.map(r => r.humidity);

        const timestamp_ascend = [...timestamps].reverse();
        const temperature_ascend = [...temperatures].reverse();
        const humidity_ascend = [...humidities].reverse();

        const hydrothermoChartData = {
            x_val: timestamp_ascend,
            y1_val: temperature_ascend,
            y2_val: humidity_ascend
        };
        $w("#chartHydrothermo").postMessage(hydrothermoChartData);
        $w('#latestRecord').text = `Latest recording at\n${timestamps[0]}\nTemperature: ${temperatures[0]} °C\nHumidity: ${humidities[0]} %`;
    } else {
        $w('#latestRecord').text = `Unable to fetch data for ${myDevice}`;
    }
}

$w.onReady(async () => {
    setTimeout(async () => {
        await loadChartData();
    }, 1000);
    $w("#refreshEnvData").onClick(async () => {
        console.log("🔄 Refresh button clicked");
        $w("#refreshEnvData").disable();
        $w('#latestRecord').text = "Fetching latest recording ...";
        await loadChartData();
        $w("#refreshEnvData").enable();
    });
});
