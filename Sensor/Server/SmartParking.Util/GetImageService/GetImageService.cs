using Emgu.CV;
using Emgu.CV.Structure;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Net.Mime.MediaTypeNames;

namespace SmartParking.Util.GetImageUtil
{
    public class GetImageService
    {
        private readonly ILogger<GetImageService> logger;
        public GetImageService(ILogger<GetImageService> logger)
        {
            this.logger = logger;
        }
        public async Task<string> ConvertImagetoString (Mat frame)
        {
            string result = string.Empty;
            try
            {
                Bitmap bitmap = frame.ToBitmap();
                MemoryStream ms = new MemoryStream();
                bitmap.Save(ms, ImageFormat.Jpeg);
                byte[] byteImage = ms.ToArray();
                result = Convert.ToBase64String(byteImage);
                return await Task.FromResult(result);
            }
            catch (Exception ex)
            {
                logger.LogError(ex.ToString());
            }
            return result;
        }
    }
}
