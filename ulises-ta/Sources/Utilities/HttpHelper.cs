using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;


namespace Utilities
{    public class HttpHelper
    {
        private static readonly HttpClient _httpClient = new HttpClient();
            
        public static async Task<string> Get(string Url)
        {
            // The actual Get method
            using (var result = await _httpClient.GetAsync(Url))
            {
                string content = await result.Content.ReadAsStringAsync();
                return content;
            }
        }

        public static async Task <string> SendPostRequest(string apiUrl, string postData)
        {
            using (HttpClient client = new HttpClient())
            {
                // Configura el contenido de la solicitud POST
                HttpContent content = new StringContent(postData, Encoding.UTF8, "application/json");

                try
                {
                    // Realiza la solicitud POST y obtiene la respuesta
                    HttpResponseMessage response = await client.PostAsync(apiUrl, content);

                    // Lee la respuesta
                    string responseBody = await response.Content.ReadAsStringAsync();

                    // Muestra la respuesta
                    //Console.WriteLine($"Status Code: {response.StatusCode}");
                    //Console.WriteLine($"Response Body: {responseBody}");
                    return response.ReasonPhrase;
                }
                catch (HttpRequestException ex)
                {
                    Console.WriteLine($"Error: {ex.Message}");
                    return null;
                }
            }
        }
    }
}
