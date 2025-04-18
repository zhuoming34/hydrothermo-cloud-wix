"""
Test posting data to AWS DynamoDB through API and Lambda
"""
import requests
import json
import time
import random

# TODO: Update api_id, region, stage, and api_name in the url below
url = f"https://{api_id}.execute-api.{region}.amazonaws.com/{stage}/{api_name}"

payload = {
    "table_name": "hydrothermo",
    "device_id": "abc",
}
headers = {"Content-Type": "application/json"}
payload["env_data"] = {}

# Test posting data
for i in range(10):
    timestamp = time.time()
    temperature = random.randint(1, 30)
    humidity = random.randint(40, 70)
    payload["timestamp"] = timestamp
    payload["env_data"]["temperature"] = temperature
    payload["env_data"]["humidity"] = humidity
    response = requests.post(url, data=json.dumps(payload), headers=headers)
    print(f"{response.status_code}: {response.json()}")