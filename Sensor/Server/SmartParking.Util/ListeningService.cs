using Microsoft.Extensions.Logging;
using MQTTnet;
using MQTTnet.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartParking.Service
{
    public class ListeningService
    {
        private readonly ILogger<ListeningService> logger;
        public ListeningService(ILogger<ListeningService> logger)
        {
            this.logger = logger;
        }
        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            try
            {
                var mqttFactory = new MqttFactory();
                IMqttClient client = mqttFactory.CreateMqttClient();
                var options = new MqttClientOptionsBuilder()
                    .WithClientId(Guid.NewGuid().ToString())
                    .WithTcpServer("192.168.1.92", 1883)
                    .WithCleanSession()
                    .Build();
                var connectResult = await client.ConnectAsync(options);
                if (connectResult.ResultCode == MqttClientConnectResultCode.Success)
                {
                    logger.LogInformation("Connected mqtt Server");
                    await client.SubscribeAsync("Sensor/RFID");

                    // Callback function when a message is received
                    client.ApplicationMessageReceivedAsync += e =>
                    {
                        logger.LogInformation($"Received message: {System.Text.Encoding.UTF8.GetString(e.ApplicationMessage.PayloadSegment)}");
                        return Task.CompletedTask;
                    };
                }
            }
            catch (Exception ex)
            {
                logger.LogError(ex.ToString());
            }
        }
    }
}
