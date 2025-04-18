import json
import boto3
from boto3.dynamodb.conditions import Key

TABLE_NAME_STR: str = "table_name"
PARTITION_KEY: str = "device_id"
SORT_KEY: str = "timestamp"
DATA_AMOUNT_STR = "data_amount"
QUERY_LIMIT_DEFAULT: int = 48
RESPONSE_BODY_NAME: str = "deviceData"


def lambda_handler(event: any, context: any):

    # Construct response object
    response = {}
    response["headers"] = {} # initially empty
    response["headers"]["Content-Type"] = "application/json"

    # Construct body of the response object
    response_body = {}

    try:
        # Get parameters
        event_params: dict = event["queryStringParameters"]
        table_name: str = event_params[TABLE_NAME_STR]
        device_id: str = event_params[PARTITION_KEY]
        
        query_limit: int = QUERY_LIMIT_DEFAULT
        data_amount: int = QUERY_LIMIT_DEFAULT
        if DATA_AMOUNT_STR in event_params:
            data_amount = int(event_params[DATA_AMOUNT_STR])
            if (data_amount > query_limit):
                query_limit = data_amount

        # Create DynanoDB client
        dynamodb = boto3.resource("dynamodb")
        table = dynamodb.Table(table_name)

        # Get the data from DynamoDB table
        table_query = table.query(
            KeyConditionExpression=Key(PARTITION_KEY).eq(device_id),
            Limit=query_limit,
            ScanIndexForward=False
        )
        device_data = table_query.get("Items", [])

        # Sort the data
        # device_data_sorted = sorted(device_data, key=lambda x: x[SORT_KEY])
        device_data_sorted = sorted(device_data, key=lambda x: x[SORT_KEY], reverse=True) # descending
        
        if (len(device_data_sorted) < data_amount):
            data_amount = len(device_data_sorted)

        response_body[RESPONSE_BODY_NAME] = device_data_sorted[:data_amount]

        response["statusCode"] = 200

    except Exception as e:
        print("Lambda error:", e)
        response["statusCode"] = 500
        response_body = {"message": "Internal server error", "error": str(e)}

    response["body"] = json.dumps(response_body, default=str) # convert to string
    return response
