#ifndef OpenSourceRF_h
#define OpenSourceRF_h

typedef struct OSRF_pkt {
  unsigned char button;
  byte esn[ESN_LEN];
} OSRF_pkt;


namespace OpenSourceRF {
	void getPacket(OSRF_pkt *rv);
}

#endif
