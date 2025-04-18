# hydrothermo-cloud-wix
A low-cost* solution to collect sensor readings, post to cloud, then display on a webpage

*We will be using free cloud services like AWS Lambda, AWS DynanoDB, Wix in this project. The cost mainly comes from AWS API Gateway, hardware components like a microcontroller board, sensors, and other parts depending on how you would like to connect sensors to the board.


**Note:**
In [AWS Free Tier](https://aws.amazon.com/free/), IoT-Core is only free for 3 months for new accounts, API Gateway is free for 12 months, Lambda and DynanomoDB remain free after the 3-month trail. 
API Gateway and Lambda provides 1 million free requests per month respectively, and DynanoDB provides 25 GB storage in total. So we will use Lambda+API Gateway to replace some functionalities we need from IoT-Core (MQTT pub/sub/rules). 
The AWS API Gateway after 12 months costs about a cup of coffee for every 1 million requests. You may consider to continue using it, set up a new AWS account, use GCP services instead, or read/write with DynamoDB directly (not recommended).

# Under Construction
