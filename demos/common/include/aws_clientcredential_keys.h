#include <stddef.h>
/*
 * PEM-encoded client certificate
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----"
 * "...base64 data..."
 * "-----END CERTIFICATE-----";
 */
static const char clientcredentialCLIENT_CERTIFICATE_PEM[] =
"-----BEGIN CERTIFICATE-----\n"
"MIIDWTCCAkGgAwIBAgIUaaXvNPQkeoBpaoatOCOOFoLrUwgwDQYJKoZIhvcNAQEL\n"
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE4MDUyMzIyMzUy\n"
"MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANuGW65Cs4WQw2OV5C9I\n"
"GvJqZVNka9QkOwxXbF8T2w/qjDdjWqmF437xTAXUu1X90pSfGYVaRzonF23VQMET\n"
"rDnr7DgCia4jCo1uh48+PB+ngXQdQBx4tbagc9Tx4yl7aIoIKGYhV3S/FridqM5i\n"
"JgeT9v0pDCFkRqsgsfBea3Jb7qnFjyEHG2HXQJsP7IWCe88hhyLvrH1iHMcW9Yla\n"
"UkBXkD011ZdN8w6/wqaHaMu9DBclEuUHJH8Knt5S4whLYOfjlB3H7goxCVXjphLQ\n"
"gF1WAgIuQVFmfMLbcBoFX6/rE3aA3ae9rXGm0/k8h5JsPpTFnDaoAIKFp2maCISY\n"
"12cCAwEAAaNgMF4wHwYDVR0jBBgwFoAUmoHnYLsMG/iorHY3myims6/b2xQwHQYD\n"
"VR0OBBYEFH6yrUtrLRTNcK2RxkzFYhziD6u6MAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCptuKYRHrfokGuLo8EZY2ZFO2z\n"
"o7rHaq0tBXyI0HutOTEO5r+vFwkGYBmGsGTpdbTATWZq826rEq30VSEE7j/Wr7we\n"
"eSoylXImmLNHEyEmVwm/bbEgEacBtROxCgaskJxRQuvtq/NciXgMbl1zopzBzD5e\n"
"bMWNbGaUe3xkZkjZLHhBV4PhyAEX4tqc4sTICy6n/Qs+jlwB8vDseoR3o9TNITL9\n"
"ufga+GYSwRYbVAt0srtWmgPGSsoJCaz1A6Em/0TZ3BX2TlKKFWTo36cglAhMXbgU\n"
"q22/XWClBLgORbYQs/i6kj/EllBt1us2cr+3hmBpvlhSsZyPvO15LLzGkney\n"
"-----END CERTIFICATE-----\n";

/*
 * PEM-encoded issuer certificate for AWS IoT Just In Time Registration (JITR).
 * This is required if you're using JITR, since the issuer (Certificate
 * Authority) of the client certificate is used by the server for routing the
 * device's initial request. (The device client certificate must always be
 * sent as well.) For more information about JITR, see:
 *  https://docs.aws.amazon.com/iot/latest/developerguide/jit-provisioning.html,
 *  https://aws.amazon.com/blogs/iot/just-in-time-registration-of-device-certificates-on-aws-iot/.
 *
 * If you're not using JITR, set the below pointer to NULL.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----"
 * "...base64 data..."
 * "-----END CERTIFICATE-----";
 */
static const char *clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

/*
 * PEM-encoded client private key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----"
 * "...base64 data..."
 * "-----END RSA PRIVATE KEY-----";
 */
static const char clientcredentialCLIENT_PRIVATE_KEY_PEM[] =
"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEogIBAAKCAQEA24ZbrkKzhZDDY5XkL0ga8mplU2Rr1CQ7DFdsXxPbD+qMN2Na\n"
"qYXjfvFMBdS7Vf3SlJ8ZhVpHOicXbdVAwROsOevsOAKJriMKjW6Hjz48H6eBdB1A\n"
"HHi1tqBz1PHjKXtoiggoZiFXdL8WuJ2ozmImB5P2/SkMIWRGqyCx8F5rclvuqcWP\n"
"IQcbYddAmw/shYJ7zyGHIu+sfWIcxxb1iVpSQFeQPTXVl03zDr/Cpodoy70MFyUS\n"
"5Qckfwqe3lLjCEtg5+OUHcfuCjEJVeOmEtCAXVYCAi5BUWZ8wttwGgVfr+sTdoDd\n"
"p72tcabT+TyHkmw+lMWcNqgAgoWnaZoIhJjXZwIDAQABAoIBAH1VS3jFektyqCtu\n"
"Q7Y++6uV1T30Yp/fA5t7OlNaOcLvjZh98CBDYexzBY16tG5XGWhjIaYCMhJgiT7a\n"
"9zqE6dqz+MzNnh++v+KZzWmaBaeo8f8CSD05Kb4YA6n7ZE+ltrzABZixxoXQJ7/F\n"
"u8ABR2ciMmy9h8B0yOghJZZvwfaquPSbk3QoTI6eck+OKJ/S1+v+hxBcO3iDiHEU\n"
"avIR1XtpjRxZzuPTEs6ZSeUJTV8nXQU3dNOxItiZjOOOXiKQcH0TviIJ4+r0+QrA\n"
"leq0/FoeDJJUORk6k69aUSCSmJ22R8sivrfR24nzR6VSGGCRQe/BtfM7Ie0rRZeL\n"
"aYPz1AECgYEA/DGOe/Ggj3IgZ8zTMXONOc5ZXUOCk6HMNEc5URbc/WSbGImmWUb4\n"
"/nHstCCkbz5x5OdTMQy+P+RzBSxhKWuh3k0/pQhqP8WIt8yrj+PjqFXkveWDoHTs\n"
"vdN9tJct0bDpj0ERDAgpctj24BneMO2sQD8iO/wHV7+3QqZcb3W85WcCgYEA3taS\n"
"4L/I9E8KiVFEs0HFsf9slCW5RmRI8Co6UkRUUyHtFPfJnmp3wyfH6/Z5XmImoxvI\n"
"pT01YiAif9wUyNshex9Xr/ZhFNd2hFhdQO3ONLGfdQ5CUnG0WnaptR1hPquJW2v/\n"
"HV17stQxGDSNAi1H4c+Yilx8tFXSWYzZI6ErPgECgYBUgx7mdiCCMZKedktHxRtM\n"
"hdP4pwj5BD10P3H7tJe16qL25Bn0zQ3f5rmV22J/TQPJ26NLSqPusVXCJA8jXEhQ\n"
"QL7VRS6l1EJupY3QmGPQI4iWAnXLraMwMdFsVUUrggei1AEscQfCzuZcTw7ggqU4\n"
"mm+Ufm7PfxWgpjNPvrrgfQKBgAXuD9qLP/3AXLdqwjlC9s/VzD3lAxkscgRyGzs1\n"
"NklADd8Q+PyOOMGovVk6JeCtG/obMyuyhUknMcPVR3hQ3IM6Qkscc1jqouzCrTid\n"
"G2+rimLDDmDDRdlibwGy1nOkfPNB3HgblL8dhnDudRIpwk/wBk8oRulFn2NirCSW\n"
"lWABAoGAFAyOZsmxLwh84+yAOHb/tL/u/QO1NEUxLg+HwRSYmDHxwrxWQ0EMWNQb\n"
"veu2Hq0+1Ec8/Re9u9q4NaNwZ3gbKvM6duJm8u8TcK7VsOvLEkxR0LiMibvpV9kL\n"
"G78ZQxcoA233PHG4z9DAHk9nEMoC1Up+co7CFpFtNiEf7o0lp8E=\n"
"-----END RSA PRIVATE KEY-----\n";
