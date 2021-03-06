// Serializable protocol for fosix syscalls
// Directly packs and unpacks from byte arrays

#pragma once

// This begs to be autogenerated...

#include "fosix.h"
#include "../aws-fpga-dma/dma_msg.h"

// Syscall table
#define syscall_t uint8_t
#define POSIX_READ  0
#define POSIX_WRITE 1
#define POSIX_OPEN  2
#define POSIX_CLOSE 3
#define POSIX_UNKNOWN 255
// Byte[0] = Sycall ID
syscall_t decode_syscall_id(dma_msg_t msg)
{
  syscall_t rv;
  rv = POSIX_UNKNOWN;
  //printf("msg.data[0] = %d\n",msg.data[0]);
  if(msg.data[0]==POSIX_READ)
  {
    rv = POSIX_READ;
  }
  else if(msg.data[0]==POSIX_WRITE)
  {
    rv = POSIX_WRITE;
  }
  else if(msg.data[0]==POSIX_OPEN)
  {
    rv = POSIX_OPEN;
  }
  else if(msg.data[0]==POSIX_CLOSE)
  {
    rv = POSIX_CLOSE;
  }
  return rv;
}
dma_msg_t apply_syscall_id(syscall_t id, dma_msg_t msg)
{
  msg.data[0] = id;
  return msg;
}

// Bytes[1+] are specific to syscall

// OPEN REQ
dma_msg_s open_req_to_dma(open_req_t req)
{
  dma_msg_s msg_stream = DMA_MSG_S_NULL();
  msg_stream.data = apply_syscall_id(POSIX_OPEN, msg_stream.data);
  msg_stream.valid = req.valid;  
  
  // Bytes[1-(PATH_SIZE-1+1)] are path
  size_t i;
  for(i=1; i<(PATH_SIZE+1); i=i+1)
  {
    msg_stream.data.data[i] = req.path[i-1];
  }
  
  return msg_stream;
}
open_req_t dma_to_open_req(dma_msg_t msg)
{
  open_req_t req;
  req.valid = decode_syscall_id(msg) == POSIX_OPEN;
  
  // Bytes[1-(PATH_SIZE-1+1)] are path
  size_t i;
  for(i=1; i<(PATH_SIZE+1); i=i+1)
  {
    req.path[i-1] = msg.data[i];
  }
  
  return req;
}

// OPEN RESP
open_resp_t dma_to_open_resp(dma_msg_s msg_stream)
{
  open_resp_t resp;
  resp.valid = (decode_syscall_id(msg_stream.data) == POSIX_OPEN) & msg_stream.valid;
  
  // Byte[1] = fildes
  resp.fildes = msg_stream.data.data[1];
  
  return resp;
}
dma_msg_s open_resp_to_dma(open_resp_t resp)
{
  dma_msg_s msg_stream = DMA_MSG_S_NULL();
  msg_stream.data = apply_syscall_id(POSIX_OPEN, msg_stream.data);
  msg_stream.valid = resp.valid;
  
  // Byte[1] = fildes
  msg_stream.data.data[1] = resp.fildes;
  
  return msg_stream;  
}

// WRITE REQ
dma_msg_s write_req_to_dma(write_req_t req)
{
  dma_msg_s msg_stream = DMA_MSG_S_NULL();
  msg_stream.data = apply_syscall_id(POSIX_WRITE, msg_stream.data);
  msg_stream.valid = req.valid;  
  
  // Byte[1] = fildes
  msg_stream.data.data[1] = req.fildes;
  // Byte[2] = nbyte
  msg_stream.data.data[2] = req.nbyte;
  // Byte[3-(BUF_SIZE-1+3)] = buf
  size_t i;
  for(i=3; i<(BUF_SIZE+3); i=i+1)
  {
    msg_stream.data.data[i] = req.buf[i-3];
  }
  
  return msg_stream;
}
write_req_t dma_to_write_req(dma_msg_t msg)
{
  write_req_t req;
  req.valid = decode_syscall_id(msg) == POSIX_WRITE;
  
  // Byte[1] = fildes
  req.fildes = msg.data[1];
  // Byte[2] = nbyte
  req.nbyte = msg.data[2];
  // Byte[3-(BUF_SIZE-1+3)] = buf
  size_t i;
  for(i=3; i<(BUF_SIZE+3); i=i+1)
  {
    req.buf[i-3] = msg.data[i];
  }
  
  return req;
}

// WRITE RESP
write_resp_t dma_to_write_resp(dma_msg_s msg_stream)
{
  write_resp_t resp;
  resp.valid = (decode_syscall_id(msg_stream.data) == POSIX_WRITE) & msg_stream.valid;
  
  // Byte[1] = nbyte
  resp.nbyte = msg_stream.data.data[1];
  
  return resp;
}
dma_msg_s write_resp_to_dma(write_resp_t resp)
{
  dma_msg_s msg_stream = DMA_MSG_S_NULL();
  msg_stream.data = apply_syscall_id(POSIX_WRITE, msg_stream.data);
  msg_stream.valid = resp.valid;
  
  // Byte[1] = nbyte
  msg_stream.data.data[1] = resp.nbyte;
  
  return msg_stream;
}

// READ REQ
dma_msg_s read_req_to_dma(read_req_t req)
{
  dma_msg_s msg_stream = DMA_MSG_S_NULL();
  msg_stream.data = apply_syscall_id(POSIX_READ, msg_stream.data);
  msg_stream.valid = req.valid;  
  
  // Byte[1] = fildes
  msg_stream.data.data[1] = req.fildes;
  // Byte[2] = nbyte
  msg_stream.data.data[2] = req.nbyte;
  
  return msg_stream;
}
read_req_t dma_to_read_req(dma_msg_t msg)
{
  read_req_t req;
  req.valid = decode_syscall_id(msg) == POSIX_READ;
  
  // Byte[1] = fildes
  req.fildes = msg.data[1];
  // Byte[2] = nbyte
  req.nbyte = msg.data[2];
  
  return req;
}

// READ RESP
read_resp_t dma_to_read_resp(dma_msg_s msg_stream)
{
  read_resp_t resp;
  resp.valid = (decode_syscall_id(msg_stream.data) == POSIX_READ) & msg_stream.valid;
  
  // Byte[1] = nbyte
  resp.nbyte = msg_stream.data.data[1];
  // Byte[2-(BUF_SIZE-1+2)] = buf
  size_t i;
  for(i=2; i<(BUF_SIZE+2); i=i+1)
  {
    resp.buf[i-2] = msg_stream.data.data[i];
  }
  
  return resp;
}
dma_msg_s read_resp_to_dma(read_resp_t resp)
{
  dma_msg_s msg_stream = DMA_MSG_S_NULL();
  msg_stream.data = apply_syscall_id(POSIX_READ, msg_stream.data);
  msg_stream.valid = resp.valid;
  
  // Byte[1] = nbyte
  msg_stream.data.data[1] = resp.nbyte;
  // Byte[2-(BUF_SIZE-1+2)] = buf
  size_t i;
  for(i=2; i<(BUF_SIZE+2); i=i+1)
  {
    msg_stream.data.data[i] = resp.buf[i-2];
  }
  
  return msg_stream;
}

// CLOSE REQ
dma_msg_s close_req_to_dma(close_req_t req)
{
  dma_msg_s msg_stream = DMA_MSG_S_NULL();
  msg_stream.data = apply_syscall_id(POSIX_CLOSE, msg_stream.data);
  msg_stream.valid = req.valid;  
  
  // Byte[1] = fildes
  msg_stream.data.data[1] = req.fildes;
  
  return msg_stream;
}
close_req_t dma_to_close_req(dma_msg_t msg)
{
  close_req_t req;
  req.valid = decode_syscall_id(msg) == POSIX_CLOSE;
  
  // Byte[1] = fildes
  req.fildes = msg.data[1];
  
  return req;
}

// CLOSE RESP
close_resp_t dma_to_close_resp(dma_msg_s msg_stream)
{
  close_resp_t resp;
  resp.valid = (decode_syscall_id(msg_stream.data) == POSIX_CLOSE) & msg_stream.valid;
  
  // Byte[1] = err
  resp.err = msg_stream.data.data[1];
  
  return resp;
}
dma_msg_s close_resp_to_dma(close_resp_t resp)
{
  dma_msg_s msg_stream = DMA_MSG_S_NULL();
  msg_stream.data = apply_syscall_id(POSIX_CLOSE, msg_stream.data);
  msg_stream.valid = resp.valid;
  
  // Byte[1] = err
  msg_stream.data.data[1] = resp.err;
  
  return msg_stream;  
}

// C2H
posix_c2h_t dma_to_request(dma_msg_t msg)
{
  posix_c2h_t req = POSIX_C2H_T_NULL();
  req.sys_open.req  = dma_to_open_req(msg);
  req.sys_write.req = dma_to_write_req(msg);
  req.sys_read.req = dma_to_read_req(msg);
  req.sys_close.req = dma_to_close_req(msg);
  return req;  
}

// H2C
dma_msg_s response_to_dma(posix_h2c_t resp)
{
  dma_msg_s msg_stream = DMA_MSG_S_NULL();
  if(resp.sys_open.resp.valid)
  {
    msg_stream = open_resp_to_dma(resp.sys_open.resp);
  }
  else if(resp.sys_write.resp.valid)
  {
    msg_stream = write_resp_to_dma(resp.sys_write.resp);
  }
  else if(resp.sys_read.resp.valid)
  {
    msg_stream = read_resp_to_dma(resp.sys_read.resp);
  }
  else if(resp.sys_close.resp.valid)
  {
    msg_stream = close_resp_to_dma(resp.sys_close.resp);
  }
  return msg_stream;
}
