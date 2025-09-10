#include "sim900.h"
#include "sim900_sms.h"
#include "sim900_http.h"
#include "sim900_sock.h"

int main(void)
{
    sim900_port_t *port;
    sim900_port_init(&port, 115200);
    sim900_init(port);
    if (sim900_power_on()) {
        sim900_at_ping(3);
        sim900_wait_registered(60000);
        sim900_bearer_open("claro.com.br", "", "", 60000);
        sim900_sms_send_text("+5511999999999", "Teste OK", 30000);
        int code; uint8_t buf[128]; size_t len = sizeof(buf);
        sim900_http_init();
        sim900_http_set_param("CID", "1");
        sim900_http_set_param("URL", "http://example.com/");
        sim900_http_do(SIM900_HTTP_GET, NULL, 0, &code, buf, &len, 60000);
        sim900_http_term();
        sim900_sock_t s; uint32_t sent; uint8_t rx[128];
        sim900_sock_attach();
        sim900_sock_open(&s, "TCP", "example.com", 80, 30000);
        const char req[] = "GET / HTTP/1.0\r\nHost: example.com\r\n\r\n";
        sim900_sock_send(s, (const uint8_t *)req, sizeof(req)-1, &sent);
        sim900_sock_recv(s, rx, sizeof(rx), 1000);
        sim900_sock_close(s);
    }
    while (1) {
        sim900_poll();
    }
    return 0;
}
