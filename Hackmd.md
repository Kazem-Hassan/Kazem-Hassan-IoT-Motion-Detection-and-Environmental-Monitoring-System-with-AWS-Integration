---
title: 'Portfolio: IoT Motion Detectio'

---

## IoT Motion Detection and Environmental Monitoring System with AWS Integration

## Introduction

This portfolio describes an Internet of Things (IoT) project aimed at detecting motion using a PIR sensor and monitoring environmental conditions using a DHT11 sensor, both integrated with an ESP32 microcontroller. The data from the sensors is transmitted to AWS IoT Core using MQTT. The project involves multiple AWS services, including DynamoDB, S3, Lambda, and SNS, to create a complete system for data collection, processing, storage, and notification. The integration with Telegram provides instant alerts when motion is detected. Grafana is used to visualize the collected sensor data.



---

## Architecture Overview
![IoT_AWS-Cloud.drawio](https://hackmd.io/_uploads/SJqZs307kg.png)

### **Hardware**


>| Material  | Description                           |Source    | Cost   |
>| --------- | ------------------------------------- |----------|--------|
>| ESP32 | Microcontroller for sensor integration and data transmission |Temu |100 Kr  |
>|DHT11 Sensor| Temperature and humidity sensor| Temu  | 10 Kr  |
>|PIR Motion Sensor	|Passive infrared sensor for motion detection|Temu|20 Kr|
>|Breadboard|For prototyping without soldering|electrokit|70 Kr|
>|Jumper Wires|Connecting components on the breadboard|electrokit|50 Kr|


### **Software and Cloud Components**


| Component                | Description |
| ------------------------ | ----------- |
|**Arduino IDE**|Used for developing and uploading code to the ESP32 microcontroller.|
| **AWS IoT Core**         | Serves as the entry point for data from the ESP32. Uses MQTT to receive messages securely via X.509 certificates. |
| **AWS Lambda**           | Processes incoming data from AWS IoT Core. Sends notifications to Telegram via the Telegram Bot API. Stores motion data in DynamoDB and S3. |
| **Amazon DynamoDB**      | Stores motion data with attributes like timestamp, device ID, and motion status. Configured with Time-to-Live (TTL) to automatically delete old records. |
| **Amazon S3**            | Stores telemetry data as JSON files for archiving and future analysis. Lifecycle rules automatically transition data to Glacier and delete after 90 days. |
| **Amazon SNS**           | Acts as an intermediary to trigger Lambda functions when motion is detected. |
| **Telegram Bot Integration** | Provides real-time motion detection alerts to a predefined Telegram chat. |
| **Grafana**              | Used for visualizing motion and environmental data from the sensors, providing insightful dashboards. |





---

## Setup and Configuration (Step-by-Step)

1. **Hardware Setup**: 

   - Connect the PIR sensor, DHT11 sensor, and ESP32 microcontroller on a breadboard.
   - Use jumper wires to connect the sensors to the appropriate pins on the ESP32.
![esp32](https://hackmd.io/_uploads/SJ93DpRm1e.jpg)

2. **Arduino IDE Configuration**:

   - Install the ESP32 board support in Arduino IDE.
   - Connect the ESP32 to your computer via USB and select the correct board and port in Arduino IDE.
   - Write and upload the code that reads data from the PIR and DHT11 sensors and sends it to AWS IoT Core.
   

3. **AWS IoT Core Setup**:

   - Create a new thing in AWS IoT Core for your ESP32 device.
   - Download the device certificates and attach an appropriate IoT policy.
   - Use these certificates in your Arduino code to securely connect to AWS IoT Core via MQTT.
   - 

4. **Lambda Configuration**:

   - Create an AWS Lambda function that handles incoming data from AWS IoT Core and prepares it for visualization in Grafana.
   - Create an AWS Lambda function that handles incoming data from AWS IoT Core and sends notifications to Telegram.
   

5. **Notification System**:

   -  Configure Amazon SNS (Simple Notification Service) as a trigger for a Lambda function that sends data to Telegram.

6. **DynamoDB and S3 Storage Setup**:

   - Create a DynamoDB table to store sensor data with attributes like timestamp and device ID.
   - Set up an S3 bucket to archive sensor data and configure lifecycle policies to manage data.

7. **Telegram Bot Integration**:
   - Create a Telegram bot using BotFather and get the API token and CHAT_ID.
   - Use the Lambda function to send motion alerts via Telegram using the Bot API.

8. **Grafana Configuration**:

   - Set up Grafana and connect it to DynamoDB visualize motion and environmental data.
   - Create dashboards for better insight into sensor activity.![Namnlöst diagram.drawio (2)](https://hackmd.io/_uploads/HJE6_ACQJg.png)


---

## Results and Achievements

- Successfully integrated hardware and cloud services for a real-time IoT solution.
- Achieved secure and reliable data transmission from ESP32 to AWS IoT Core.
- Implemented automated data management using TTL and S3 lifecycle policies.
- Deployed a Telegram bot for instant motion detection alerts.![telegram](https://hackmd.io/_uploads/S17ZI00Xkg.jpg)

- Visualized telemetry data effectively using Grafana.![Grafana](https://hackmd.io/_uploads/r1PQUR0m1x.jpg)



---

## Future Improvements

1. **Add More Sensors**:

   - Integrate additional sensors like distance (HC-SR04) and cammera .
  

2. **Edge Processing**:

   - Utilize ESP32 for basic data filtering like object recognition and face recognition to reduce cloud processing.

3. **Scalability**:

   - Extend the system to handle multiple devices and locations.
   -  This involves setting up additional ESP32 devices, each configured with unique identifiers to send data to AWS IoT Core.
   -   AWS IoT Core can manage multiple devices through device management features, allowing scalability across different locations.
   -    The infrastructure can be scaled by adding more DynamoDB tables or partitions to handle increased data volumes, and S3 buckets can be expanded for long-term storage of telemetry data. Additionally, using AWS IoT Fleet Management helps keep track of all connected devices, making the system easier to monitor and maintain.

4. **Data Analytics**:

   - Use AWS Athena or QuickSight to analyze sensor data trends. Additionally, leverage AI and machine learning services, such as AWS SageMaker, to build predictive models that provide insights into sensor data patterns and detect anomalies in real-time.

5. **Enhanced Visualization Control**:
    - Use AWS Amplify to provide more control over data visualization, allowing customization of dashboards to filter and display data only for specific device owners.


---



## Skills Demonstrated

- IoT hardware integration (ESP32, PIR sensor, DHT11 sensor).
- Secure communication with MQTT over TLS.
- AWS services: IoT Core, Lambda, DynamoDB, S3, SNS.
- Serverless architecture design.
- Real-time notification systems (Telegram bot).

---

## Conclusion

This project demonstrates the successful integration of IoT hardware with AWS services to build a real-time motion detection system. By leveraging AWS services like IoT Core, DynamoDB, S3, and Lambda, the project provides a scalable and cost-effective solution for motion detection and notification. The inclusion of lifecycle policies and visualization ensures efficient data management and usability.


