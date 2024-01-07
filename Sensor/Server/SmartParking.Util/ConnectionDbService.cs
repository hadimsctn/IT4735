using Microsoft.Extensions.Logging;
using MongoDB.Driver;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace SmartParking.Service
{
    public class ConnectionDbService
    {
        private readonly ILogger<ConnectionDbService> logger;
        public ConnectionDbService(ILogger<ConnectionDbService> logger)
        {
            this.logger = logger;
        }
        public Task<bool> ConnectToMongoDB()
        {
            MongoClient dbClient = new MongoClient("mongodb+srv://hanguyenduc2002:12345678@smartparking.qjhpal7.mongodb.net/");
            var dbList = dbClient.ListDatabases().ToList();

            logger.LogInformation("The list of databases on this server is: ");
            foreach (var db in dbList)
            {
                logger.LogInformation(db.ToString());
            }
            return Task.FromResult(true);
        }
    }
}
