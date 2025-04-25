# AWS Lambda Functions
This folder contains two examples of Lambda functions in Python to interact with DynamoDB, and two associate code to trigger and test the Lambda function through API Gateway. 

In Python, we can use the [Boto3](https://boto3.amazonaws.com/) (AWS Python SDK) to communicate with AWS services like DynamoDB, SQS, SNS, S3, EC2, etc.

In AWS Lambda, Boto3 is pre-installed and available without additional configurations.

## [post_to_db.py](./post_to_db.py)
A Python example to post/write new data to a DynamoDB table.

#### [try_post_api.py](./try_post_api.py)
A Python example to demonstrate how to invoke the POST API and pass data to Lambda function.

## [get_from_db.py](./get_from_db.py)
A Python example to get/read a specific amount of data from a DynamoDB table, then return them in the response object.

#### [try_get_api.py](./try_get_api.py)
A Python example to demonstrate how to invoke the GET API and how the data returned by the Lambda function look like.
