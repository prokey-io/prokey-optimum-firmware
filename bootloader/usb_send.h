static void send_msg_success(usbd_device *dev) {
  uint8_t response[64];
  memzero(response, sizeof(response));
  // response: Success message (id 2), payload len 0
  memcpy(response,
         // header
         "?##"
         // msg_id
         "\x00\x02"
         // msg_size
         "\x00\x00\x00\x00",
         9);
  while (usbd_ep_write_packet(dev, ENDPOINT_ADDRESS_IN, response, 64) != 64) {
  }
}

static void send_msg_failure(usbd_device *dev) {
  uint8_t response[64];
  memzero(response, sizeof(response));
  // response: Failure message (id 3), payload len 2
  //           - code = 99 (Failure_FirmwareError)
  memcpy(response,
         // header
         "?##"
         // msg_id
         "\x00\x03"
         // msg_size
         "\x00\x00\x00\x02"
         // data
         "\x08"
         "\x63",
         11);
  while (usbd_ep_write_packet(dev, ENDPOINT_ADDRESS_IN, response, 64) != 64) {
  }
}

static void sendMsgFailureWithReason(usbd_device *dev, uint8_t reason)
{
  uint8_t response[64];
  memzero(response, sizeof(response));

	memcpy(response, 
		// header
		"?##"
		// msg_id
		"\x00\x03"
		// msg_size
		"\x00\x00\x00\x02"
		// data
		"\x08",
		 10);
		
	response[10] = reason;

	while ( usbd_ep_write_packet(dev, ENDPOINT_ADDRESS_IN, response, 64) != 64) {}
}

static void send_msg_features(usbd_device *dev) {
  uint8_t response[64];
  memzero(response, sizeof(response));
  memcpy(response,
         // header
         "?##"
         // msg_id
         "\x00\x11"
         // msg_size
         "\x00\x00\x00\x16"
         // data
         "\x0a"
         "\x09"
         "prokey.io"
         "\x10" VERSION_MAJOR_CHAR "\x18" VERSION_MINOR_CHAR
         "\x20" VERSION_PATCH_CHAR
         "\x28"
         "\x01"
         "\x90\x01"
         "\x00"
         "\xaa"
         "\x01"
         "1",
         34);
         // TODO: Firmware present
  response[30] = firmware_present() ? 0x01 : 0x00;
  while (usbd_ep_write_packet(dev, ENDPOINT_ADDRESS_IN, response, 64) != 64) {
  }
}

static void SendPacketToUsb( usbd_device *dev, unsigned short msgId, unsigned char* data, unsigned char dataLenght )
{
	unsigned char buffer[64];
	unsigned int n=0;

  memset(buffer, 0, 64);

  //! Start bytes
	buffer[n++] = '?';
	buffer[n++] = '#';
	buffer[n++] = '#';

  //! Message ID
	buffer[n++] = (msgId >> 8) & 0xFF;
	buffer[n++] = msgId & 0xFF;

  //! Data lenght
	buffer[n++] = (dataLenght >> 24) & 0xFF;
	buffer[n++] = (dataLenght >> 16) & 0xFF;
	buffer[n++] = (dataLenght >> 8) & 0xFF;
	buffer[n++] = dataLenght & 0xFF;

	for(int i=0; i<dataLenght; i++)
	{
		buffer[n++] = data[i];

    //! Send packets of 64 bytes
		if(n == 64)
		{
			while ( usbd_ep_write_packet(dev, ENDPOINT_ADDRESS_IN, buffer, 64) != 64 ) {}
			memset(buffer, 0, 64);
      buffer[0] = '?';
      n = 1;
		}
	}

  //! Send the rest of data
	if(n > 1)
	{
		while ( usbd_ep_write_packet(dev, ENDPOINT_ADDRESS_IN, buffer, 64) != 64 ) {}
	}
}
