import json
import boto3
from decimal import Decimal

TABLE_NAME_STR: str = "table_name"
DEVICE_ID_STR: str = "device_id"
TIMESTAMP_STR: str = "timestamp"
ENV_DATA_STR: str = "env_data"

def convert_floats_to_decimal(obj):
    if isinstance(obj, float):
        return Decimal(str(obj))  # str() to avoid floating-point inaccuracies
    elif isinstance(obj, dict):
        return {k: convert_floats_to_decimal(v) for k, v in obj.items()}
    elif isinstance(obj, list):
        return [convert_floats_to_decimal(i) for i in obj]
    else:
        return obj

def lambda_handler(event: any, context: any):

    # Construct response object
    response_body = {}
    response = {
        "headers": {
            "Content-Type": "application/json"
        }
    }
   
    try:
        body = json.loads(event['body'])
        table_name = body.get(TABLE_NAME_STR)
        device_id = body.get(DEVICE_ID_STR)
        timestamp = body.get(TIMESTAMP_STR)
        env_data: dict = body.get(ENV_DATA_STR)  
        env_data = convert_floats_to_decimal(env_data) 
        
        # Create DynanoDB client
        dynamodb = boto3.resource("dynamodb")
        table = dynamodb.Table(table_name)

        item = {
            DEVICE_ID_STR: device_id,
            TIMESTAMP_STR: int(timestamp),
            ENV_DATA_STR: env_data
        }

        put_response = table.put_item(Item=item)

        response["statusCode"] = put_response["ResponseMetadata"]["HTTPStatusCode"]
        response_body = {"message": f"Added new item to table {table_name}"}

    except Exception as e:
        print("Lambda error:", e)
        response["statusCode"] = 500
        response_body = {"message": "Internal server error", "error": str(e)}

    response["body"] = json.dumps(response_body, default=str) # convert to string
    return response