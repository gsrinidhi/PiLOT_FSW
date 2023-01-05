

#ifndef _CCSDS_H
#define _CCSDS_H
#define Version 0b0
#define tlm_pkt_type 0b0
#define cmd_pkt_type 0b1
#define shf 0b1
#define seq_flg 0b10  // as our packet's last segment is of user data
#define tlm_head_mask 0b0000

#define ccsds_p1(pkt_type, api) (uint16_t)( (tlm_head_mask << 4) | (Version << 13) | (pkt_type << 12) | (shf << 11) | api)
#define ccsds_p2(pkt_seq_count) (uint16_t)( (seq_flg << 14) | (pkt_seq_count) )
#define ccsds_p3(pkt_len) (uint16_t)(pkt_len)

#endif


