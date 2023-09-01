console.log("Loading function");
const aws = require("aws-sdk");

const docClient = new aws.DynamoDB.DocumentClient();

exports.handler = async (event, context) => {
  //console.log('Received event:', JSON.stringify(event, null, 2));
  for (const record of event.Records) {
    console.log(record.eventID);
    console.log(record.eventName);
    console.log("DynamoDB Record: %j", record.dynamodb);
    if (
      record.dynamodb["OldImage"] === undefined ||
      record.dynamodb["NewImage"] === undefined
    ) {
      continue;
    }
    console.log(record.dynamodb["NewImage"]);
    var oldHealth = record.dynamodb["OldImage"]["Health"]["N"];

    var newHealth = record.dynamodb["NewImage"]["Health"]["N"];
    var msg = "Health of plant changed from " + oldHealth + " to " + newHealth;
    var dbparams = {
      TableName: "dynamo89ef77d7-dev",
      Item: {
        // dummy value
        UserId: aws.util.uuid.v4(),

        userId: "123",
        message: msg,
      },
    };
    console.log(dbparams);
    try {
      await docClient.put(dbparams).promise();
    } catch (err) {
      console.log(err);
    }
    return "success";
  }

  return `Successfully processed ${event.Records.length} records.`;
};
