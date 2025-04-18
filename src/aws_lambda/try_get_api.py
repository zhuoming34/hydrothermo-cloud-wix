"""
Test getting data from AWS DynamoDB through API and Lambda
"""
import requests

# TODO: Update api_id, region, stage, and api_name in the url below
url = f"https://{api_id}.execute-api.{region}.amazonaws.com/{stage}/{api_name}"

response = requests.get(
    url,
    params={
        "table_name":"hydrothermo", # <--- UPDATE THIS
        "device_id":"abc", # <--- UPDATE THIS
        # "data_amount": 100
        }
)

print(response.json())