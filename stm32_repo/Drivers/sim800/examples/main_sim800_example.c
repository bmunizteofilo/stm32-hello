#include "Drivers/usart.h"
#include "sim800.h"
#include "sim800_sms.h"
#include "sim800_http.h"
#include "sim800_sock.h"
#include "sim800_dbg.h"

static void on_sms(int idx, const char *mem) { SIM800_LOGI("SMS novo idx=%d mem=%s", idx, mem); }
static void on_call(const char *num) { SIM800_LOGI("Chamada de %s", num); }
static void on_ussd(const char *t, int dcs) { SIM800_LOGI("USSD(dcs=%d): %s", dcs, t); }

int main(void) {
    sim800_port_t *port = NULL;
    uint8_t resp[1024];
    size_t len = sizeof(resp);
    int http_code = 0;
    sim800_sock_t s;

    sim800_port_init(&port, 115200);
    sim800_init(port);
    sim800_set_on_sms(on_sms);
    sim800_set_on_call(on_call);
    sim800_set_on_ussd(on_ussd);

    if (!sim800_at_ping(5)) {
        SIM800_LOGE("AT ping falhou");
    }

    if (!sim800_wait_registered(60000)) {
        SIM800_LOGW("Nao registrou na rede em 60s, seguindo mesmo assim (demo).");
    }

    if (!sim800_bearer_open("claro.com.br", "", "", 60000)) {
        SIM800_LOGE("Falha ao abrir bearer");
    }

    sim800_sms_set_text_mode(true);
    sim800_sms_send_text("+5500000000", "SIM800 OK", 30000);

    sim800_http_init();
    sim800_http_set_param("CID","1");
    sim800_http_set_param("URL","http://example.com/");
    len = sizeof(resp);
    if (sim800_http_do(SIM800_HTTP_GET, NULL, 0, &http_code, resp, &len, 60000)) {
        SIM800_LOGI("HTTP %d, %u bytes", http_code, (unsigned)len);
    } else {
        SIM800_LOGE("HTTP GET falhou");
    }
    sim800_http_term();

    if (sim800_sock_attach()) {
        if (sim800_sock_open(&s,"TCP","example.com",80,30000)) {
            const char *req = "GET / HTTP/1.0\r\nHost: example.com\r\n\r\n";
            uint32_t sent = 0;
            sim800_sock_send(s,(const uint8_t*)req,strlen(req),&sent);
            int r = sim800_sock_recv(s, resp, sizeof(resp), 20000);
            SIM800_LOGI("Socket recv=%d", r);
            sim800_sock_close(s);
        } else {
            SIM800_LOGE("CIPSTART falhou");
        }
        sim800_sock_detach();
    }

    for (;;) {
        sim800_poll();
    }
}
