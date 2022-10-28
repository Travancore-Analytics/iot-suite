#ifndef IOT_FLASHSTORAGE_H
#define IOT_FLASHSTORAGE_H

#include <Arduino.h>

#define CAT_NX(A, B) A ## B
#define CAT(A, B) CAT_NX(A, B)

#if defined(__SAMD51__)
  #define IoTFlash(name, size) \
  __attribute__((__aligned__(8192))) \
  static const uint8_t CAT(_data,name)[(size+8191)/8192*8192] = { }; \
  IoTFlashClass name(CAT(_data,name), size);

#define IoTFlashStorage(name, T) \
  __attribute__((__aligned__(8192))) \
  static const uint8_t CAT(_data,name)[(sizeof(T)+8191)/8192*8192] = { }; \
  IoTFlashStorageClass<T> name(CAT(_data,name));
#else
  #define IoTFlash(name, size) \
  __attribute__((__aligned__(256))) \
  static const uint8_t CAT(_data,name)[(size+255)/256*256] = { }; \
  IoTFlashClass name(CAT(_data,name), size);

#define IoTFlashStorage(name, T) \
  __attribute__((__aligned__(256))) \
  static const uint8_t CAT(_data,name)[(sizeof(T)+255)/256*256] = { }; \
  IoTFlashStorageClass<T> name(CAT(_data,name));
#endif

class IoTFlashClass 
{
public:
  IoTFlashClass(const void *mem_addr = NULL, uint32_t size = 0);

  void write(const void *data) { write(mem_address, data, mem_size); }
  void erase()                 { erase(mem_address, mem_size);       }
  void read(void *data)        { read(mem_address, data, mem_size);  }

  void write(const volatile void *mem_ptr, const void *data_ptr, uint32_t size);
  void erase(const volatile void *mem_ptr, uint32_t size);
  void read(const volatile void *mem_ptr, void *data_ptr, uint32_t size);

private:
  void erase(const volatile void *mem_ptr);

  const uint32_t PAGE_SIZE, PAGES, MAX_FLASH, ROW_SIZE;
  const volatile void *mem_address;
  const uint32_t mem_size;
};

template<class T>
class IoTFlashStorageClass 
{
public:
  IoTFlashStorageClass(const void *mem_addr) : memory(mem_addr, sizeof(T)) { };

  inline void write(T data)  { memory.erase(); memory.write(&data); }

  inline void read(T *data)  { memory.read(data); }

  inline T read() { T data; read(&data); return data; }

private:
  IoTFlashClass memory;
};

#endif
