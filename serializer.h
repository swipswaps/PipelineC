#pragma once
#include "arrays.h"

// INSIZE must be divisible by OUTSIZE ??
#define serializer_in_to_out(name, data_t, IN_SIZE, OUT_SIZE) \
typedef struct name##_t \
{ \
  data_t out_data[OUT_SIZE]; \
  uint1_t out_data_valid; \
  uint1_t in_data_ready; \
}name##_t; \
name##_t name(data_t in_data[IN_SIZE], uint1_t in_data_valid, uint1_t out_data_ready) \
{ \
  static data_t in_buffer[IN_SIZE]; \
  static uint1_t in_buffer_valid; \
  static uint32_t out_counter; \
  name##_t rv; \
  uint32_t i; \
  /* Default outputs from front/bottom of shift buffer */ \
  for(i=0;i<OUT_SIZE;i+=1) \
  { \
    rv.out_data[i] = in_buffer[i]; \
  } \
  rv.out_data_valid = in_buffer_valid; \
  /* Read if buffer empty */ \
  rv.in_data_ready = !in_buffer_valid; \
  \
  /* Only shift buffer if output ready */ \
  if(out_data_ready) \
  { \
    /* Shift buffer down to bring next elems to front */ \
    ARRAY_SHIFT_DOWN(in_buffer,IN_SIZE,OUT_SIZE,i) \
    out_counter += OUT_SIZE; \
    \
    /* If output all the elems then clear buffer */ \
    if(out_counter==IN_SIZE) \
    { \
      in_buffer_valid = 0; \
    } \
  } \
  \
  /* Input registers */ \
  if(rv.in_data_ready) \
  { \
    in_buffer = in_data; \
    in_buffer_valid = in_data_valid; \
    out_counter = 0; \
  } \
  \
  return rv; \
}


#define type_byte_serializer(name, in_t, OUT_BYTES) \
/* Need to ser from byte array*/ \
serializer_in_to_out(name##_serializer_in_to_out, uint8_t, sizeof(in_t), OUT_BYTES) \
typedef struct name##_t \
{ \
  uint8_t out_data[OUT_BYTES]; \
  uint1_t valid; \
  uint1_t in_data_ready; \
} name##_t; \
name##_t name(in_t in_data, uint1_t in_data_valid, uint1_t out_data_ready) \
{ \
  /* Convert type to byte array */ \
  in_t##_bytes_t input_bytes = in_t##_to_bytes(in_data); \
  /* Serialize byte array */ \
  name##_t o; \
  name##_serializer_in_to_out_t to_bytes = name##_serializer_in_to_out(input_bytes.data, in_data_valid, out_data_ready); \
  o.out_data = to_bytes.out_data; \
  o.valid = to_bytes.out_data_valid; \
  o.in_data_ready = to_bytes.in_data_ready; \
  return o; \
}


// TODO replace with in to out version? always use array args?
#define serializer(name, data_t, IN_SIZE) \
data_t name##_in_buffer[IN_SIZE]; \
uint1_t name##_in_buffer_valid; \
uint32_t name##_out_counter; \
typedef struct name##_o_t \
{ \
  data_t out_data; \
  uint1_t out_data_valid; \
  uint1_t in_data_ready; \
}name##_o_t; \
name##_o_t name(data_t in_data[IN_SIZE], uint1_t in_data_valid, uint1_t out_data_ready) \
{ \
  name##_o_t rv; \
  /* Default outputs from front of shift buffer */ \
  rv.out_data = name##_in_buffer[0]; \
  rv.out_data_valid = name##_in_buffer_valid; \
  /* Read if buffer empty */ \
  rv.in_data_ready = !name##_in_buffer_valid; \
  \
  /* Only shift buffer if output ready */ \
  if(out_data_ready) \
  { \
    /* Shift buffer to bring next elem to front */ \
    uint32_t i; \
    for(i=0;i<(IN_SIZE-1);i=i+1) \
    { \
      name##_in_buffer[i] = name##_in_buffer[i+1]; \
    } \
    name##_out_counter += 1; \
    \
    /* If output all the elems then clear buffer */ \
    if(name##_out_counter==IN_SIZE) \
    { \
      name##_in_buffer_valid = 0; \
    } \
  } \
  \
  /* Input registers */ \
  if(rv.in_data_ready) \
  { \
    name##_in_buffer = in_data; \
    name##_in_buffer_valid = in_data_valid; \
    name##_out_counter = 0; \
  } \
  \
  return rv; \
} 
