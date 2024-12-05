
const https = require("https");



const TELEGRAM_TOKEN = process.env.TELEGRAM_TOKEN;
const CHAT_ID = process.env.CHAT_ID;



exports.handler = async (event) => {
    try {
        console.log("Incoming event:", JSON.stringify(event, null, 2));

        if (event.motionDetected) {
            const telegramMessage = "🚨 Rörelse upptäckt i ditt hus! 🚨";

            const options = {
                hostname: "api.telegram.org",
                path: `/bot${TELEGRAM_TOKEN}/sendMessage`,
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                },
            };

            const requestBody = JSON.stringify({
                chat_id: CHAT_ID,
                text: telegramMessage,
            });

            const response = await new Promise((resolve, reject) => {
                const req = https.request(options, (res) => {
                    let data = "";
                    res.on("data", (chunk) => (data += chunk));
                    res.on("end", () => resolve({ statusCode: res.statusCode, body: data }));
                });

                req.on("error", (e) => reject(e));
                req.write(requestBody);
                req.end();
            });

            console.log("Response from Telegram:", response);

            return {
                statusCode: response.statusCode,
                body: response.body,
            };
        } else {
            console.log("No motion detected, skipping Telegram notification.");
            return {
                statusCode: 200,
                body: "No motion detected, no message sent.",
            };
        }
    } catch (error) {
        console.error("Error:", error.message);
        return {
            statusCode: 500,
            body: JSON.stringify(error.message),
        };
    }
};
