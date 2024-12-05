import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import {
  DynamoDBDocumentClient,
  ScanCommand,
} from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({});
const dynamo = DynamoDBDocumentClient.from(client);

const tableName = process.env.tableName;
const allowedHeader = process.env.allowedHeader;

export const handler = async (event) => {
  console.log("Incoming Event:", event);

  let body = {};
  let statusCode = 401;

  if (event.headers?.authorization == allowedHeader) {
    if (event.requestContext.http.method == "GET") {
      switch (event.requestContext.http.path) {
        case "/items":
          try {
            if (event.queryStringParameters?.startTs && event.queryStringParameters?.endTs) {
              const { startTs, endTs } = event.queryStringParameters;
              console.log("tsSpan:", startTs, endTs);

              const params = {
                TableName: tableName,
                FilterExpression: '#timestamp BETWEEN :startTs AND :endTs',
                ExpressionAttributeNames: {
                  '#timestamp': 'timestamp',
                },
                ExpressionAttributeValues: {
                  ':startTs': startTs,
                  ':endTs': endTs,
                },
              };

              body = await dynamo.send(new ScanCommand(params));
            } else {
              body = await dynamo.send(
                new ScanCommand({ TableName: tableName })
              );
            }

            if (body.Items) {
              body = body.Items.map(item => {
                const { owner, ...filteredItem } = item;
                return filteredItem;
              });
            } else {
              body = [];
            }

            statusCode = 200;
          } catch (error) {
            console.error("Error fetching data from DynamoDB:", error);
            statusCode = 500;
            body = { error: "Internal Server Error" };
          }
          break;

        default:
          statusCode = 404;
          body = { error: "Not Found" };
          break;
      }
    }
  }

  return {
    statusCode,
    body: JSON.stringify(body),
  };
};
