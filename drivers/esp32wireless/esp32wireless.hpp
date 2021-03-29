#pragma once


#include <string.h>
#include "pico/stdlib.h"
#include "spi_drv.hpp"




#define WL_FW_VER_LENGTH 6

#define WIFI_SPI_ACK        1
#define WIFI_SPI_ERR        0xFF

// Maxmium number of socket
#define	WIFI_MAX_SOCK_NUM	10
// Socket not available constant
#define SOCK_NOT_AVAIL  255
// Default state value for Wifi state field
#define NA_STATE -1
//Maximum number of attempts to establish wifi connection
#define WL_MAX_ATTEMPT_CONNECTION	10

enum wl_ping_result_t {
  WL_PING_DEST_UNREACHABLE = -1,
  WL_PING_TIMEOUT = -2,
  WL_PING_UNKNOWN_HOST = -3,
  WL_PING_ERROR = -4
};

enum wl_status_t {
	WL_NO_SHIELD = 255,
  WL_NO_MODULE = WL_NO_SHIELD,
  WL_IDLE_STATUS = 0,
  WL_NO_SSID_AVAIL,
  WL_SCAN_COMPLETED,
  WL_CONNECTED,
  WL_CONNECT_FAILED,
  WL_CONNECTION_LOST,
  WL_DISCONNECTED,
  WL_AP_LISTENING,
  WL_AP_CONNECTED,
  WL_AP_FAILED
};

enum wl_enc_type {  /* Values map to 802.11 encryption suites... */
    ENC_TYPE_WEP  = 5,
    ENC_TYPE_TKIP = 2,
    ENC_TYPE_CCMP = 4,
    /* ... except these two, 7 and 8 are reserved in 802.11-2007 */
    ENC_TYPE_NONE = 7,
    ENC_TYPE_AUTO = 8,

    ENC_TYPE_UNKNOWN = 255
};

enum wl_error_code_t {
    WL_FAILURE = -1,
    WL_SUCCESS = 1,
};



#define KEY_IDX_LEN     1



namespace pimoroni {

  struct IPAddress {
      uint32_t a;
  };

  class WifiNINA {
    //--------------------------------------------------
    // Constants
    //--------------------------------------------------
  public:


    //--------------------------------------------------
    // Variables
    //--------------------------------------------------
  private:
    SpiDrv driver;

    char 	network_ssid[WL_NETWORKS_LIST_MAXNUM][WL_SSID_MAX_LENGTH];

  	// firmware version string in the format a.b.c
	  char 	fwVersion[WL_FW_VER_LENGTH];

	  // settings of current selected network
	  char 	_ssid[WL_SSID_MAX_LENGTH];
	  uint8_t 	_bssid[WL_MAC_ADDR_LENGTH];
	  uint8_t 	_mac[WL_MAC_ADDR_LENGTH];
	  uint8_t  _localIp[WL_IPV4_LENGTH];
	  uint8_t  _subnetMask[WL_IPV4_LENGTH];
	  uint8_t  _gatewayIp[WL_IPV4_LENGTH];
  public:
    //PicoWireless();


    //--------------------------------------------------
    // Methods
    //--------------------------------------------------
  public:
    virtual bool init();

    //--------------------------------------------------
    //From https://github.com/adafruit/WiFiNINA/blob/master/src/utility/wifi_drv.cpp
    //--------------------------------------------------

    void get_network_data(uint8_t *ip, uint8_t *mask, uint8_t *gwip);
    void get_remote_data(uint8_t sock, uint8_t *ip, uint8_t *port);

    int8_t wifi_set_network(const char* ssid, uint8_t ssid_len);
    int8_t wifi_set_passphrase(const char* ssid, uint8_t ssid_len, const char *passphrase, const uint8_t len);
    int8_t wifi_set_key(const char* ssid, uint8_t ssid_len, uint8_t key_idx, const void *key, const uint8_t len);

    void config(uint8_t valid_params, uint32_t local_ip, uint32_t gateway, uint32_t subnet);

    void set_dns(uint8_t valid_params, uint32_t dns_server1, uint32_t dns_server2);
    void set_hostname(const char* hostname);
    int8_t disconnect();

    uint8_t get_connection_status();
    uint8_t* get_mac_address();

    void get_ip_address(uint8_t *ip_out);
    void get_subnet_mask(uint8_t *mask_out);
    void get_gateway_ip(uint8_t *ip_out);

    const char* get_current_ssid();
    uint8_t* get_current_bssid();
    int32_t get_current_rssi();
    uint8_t get_current_encryption_type();

    int8_t start_scan_networks();
    uint8_t get_scan_networks();
    const char* get_ssid_networks(uint8_t network_item);

    uint8_t get_enc_type_networks(uint8_t network_item);
    uint8_t* get_bssid_networks(uint8_t network_item, uint8_t* bssid_out);
    uint8_t get_channel_networks(uint8_t network_item);
    int32_t get_rssi_networks(uint8_t network_item);

    uint8_t req_host_by_name(const char* hostname);
    int get_host_by_name(IPAddress& aResult); //TODO
    int get_host_by_name(const char* aHostname, IPAddress& aResult); //Calls req_host_by_name and get_host_by_name

    const char* get_fw_version();
    uint32_t get_time();
    void set_power_mode(uint8_t mode);

    int8_t wifi_set_ap_network(const char* ssid, uint8_t ssid_len, uint8_t channel);
    int8_t wifi_set_ap_passphrase(const char* ssid, uint8_t ssid_len, const char *passphrase, const uint8_t len, uint8_t channel);

    int16_t ping(uint32_t ip_address, uint8_t ttl);

    void debug(uint8_t on);
    float get_temperature();
    void pin_mode(uint8_t pin, uint8_t mode);

    void digital_write(uint8_t pin, uint8_t value);
    void analog_write(uint8_t pin, uint8_t value);


    //--------------------------------------------------
    //From https://github.com/adafruit/WiFiNINA/blob/master/src/utility/server_drv.cpp
    //--------------------------------------------------

    void start_server(uint16_t port, uint8_t sock, uint8_t prot_mode);
    void start_server(uint32_t ip_address, uint16_t port, uint8_t sock, uint8_t prot_mode);
    void start_client(uint32_t ip_address, uint16_t port, uint8_t sock, uint8_t prot_mode);
    void start_client(const char* host, uint8_t host_len, uint32_t ip_address, uint16_t port, uint8_t sock, uint8_t prot_mode);
    void stop_client(uint8_t sock);
    
    uint8_t get_server_state(uint8_t sock);
    uint8_t get_client_state(uint8_t sock);
    uint16_t avail_data(uint8_t sock);
    uint8_t avail_server(uint8_t sock);

    bool get_data(uint8_t sock, uint8_t *data_out, uint8_t peek);
    bool get_data_buf(uint8_t sock, uint8_t *data_out, uint16_t *data_len_out);
    bool insert_data_buf(uint8_t sock, const uint8_t *data_in, uint16_t len);
    bool send_udp_data(uint8_t sock);

    uint16_t send_data(uint8_t sock, const uint8_t *data_in, uint16_t len);
    uint8_t check_data_sent(uint8_t sock);
    uint8_t get_socket();

    //--------------------------------------------------
    // TODO Sort below ones out, particularly wpa2s from CPy
    //--------------------------------------------------

    //Unknown how to implement these
    // uint8_t digital_read(uint8_t pin);
    // uint8_t analog_read(uint8_t pin);


    // void wifi_set_entidentity();    //SET_ENT_IDENT
    // void wifi_set_entusername();    //SET_ENT_UNAME
    // void wifi_set_entpassword();    //SET_ENT_PASSWD

    // void wifi_set_entenable();      //SET_ENT_ENABLE

    // void is_connected(void);     //Reads an internal status var for WL_CONNECTED

    // void connect(void);         //Calls connect_ap
    // void connect_ap(void);      //Calls wifi_set_network and wifi_set_passphrase
  };

}