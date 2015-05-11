#ifndef GAME_ENGINE_MEMORY_UTILS_H
#define GAME_ENGINE_MEMORY_UTILS_H

#pragma region CHANGE LOG
/// -- January, 31, 2015 - Nathan Hanlan - Added class/file MemoryUtils
#pragma endregion

#include <new>
#include "AllocatorType.h"
#include "MemoryFlags.h"
#include "MemoryHeader.h"

namespace Engine
{
    namespace Memory
    {
        class MemoryUtils
        {
        public:
            inline static void Free(void * aMemory)
            {
                free(aMemory);
            }

            inline static void * Malloc(unsigned int aSize)
            {
                return malloc(aSize);
            }

            ///This method will calculate the alignment needed for an address and return the aligned address
            ///Eg...
            inline static void * AlignForward(void * aAddress, unsigned char aAlignment)
            {
                return (void*)((reinterpret_cast<uintptr_t>(aAddress)+static_cast<uintptr_t>(aAlignment - 1)) &  static_cast<uintptr_t>(~(aAlignment - 1)));
            }
            ///This method will calculate the alignment needed for an address and return the adjustment required to align the address
            ///Eg...
            inline static uintptr_t AlignForwardAdjustment(void * aAddress, unsigned char aAlignment)
            {
                uintptr_t adjustment = aAlignment - (reinterpret_cast<uintptr_t>(aAddress)& static_cast<uintptr_t>(aAlignment - 1));
                if (adjustment == aAlignment)
                {
                    return 0;
                }
                return adjustment;
            }
            ///This method will calculate the alignment needed for an address including the size of the header. This is useful for adding additional data overhead to allocations.
            ///This method will return the adjustment to make.
            ///Eg...
            inline static uintptr_t AlignForwardAdjustmentHeader(void * aAddress, unsigned char aAlignment, unsigned char aHeaderSize)
            {
                uintptr_t adjustment = AlignForwardAdjustment(aAddress, aAlignment);
                uintptr_t size = aHeaderSize;
                if (adjustment < size)
                {
                    size -= adjustment;
                    adjustment += aAlignment * (size / aAlignment);
                    if (size % aAlignment > 0)
                    {
                        adjustment += aAlignment;
                    }
                }
                return adjustment;
            }
            ///Increase the pointer address aCount address spaces
            inline static void * AddPtr(void * aAddress, unsigned int aCount)
            {
                return (void*)(reinterpret_cast<uintptr_t>(aAddress)+aCount);
            }
            ///Decrease the pointer address aCount address spaces
            inline static void * SubtractPtr(void * aAddress, unsigned int aCount)
            {
                return (void*)(reinterpret_cast<uintptr_t>(aAddress)-aCount);
            }

                        
            /// <summary>
            /// Converts an object size to an index to be used with static pool allocators inside the MemoryManager class.
            /// </summary>
            /// <param name="aObjectSize">The size of the object to convert.</param>
            /// <returns>Returns an index corresponding to the size. Returns -1 if the size was invalid.</returns>
            inline static SInt8 ConvertSizeToIndex(UInt32 aObjectSize)
            {
                if (aObjectSize == 0)
                {
                    return -1;
                }
                //Split Half
                if (aObjectSize < 33)
                {
                    if (aObjectSize < 17)
                    {
                        if (aObjectSize < 9)
                        {
                            if (aObjectSize < 5)
                            {
                                return 0; //4 bytes
                            }
                            else
                            {
                                return 1; //8 bytes;
                            }
                        } // End 4/8
                        else
                        {
                            if (aObjectSize < 13)
                            {
                                return 2; //12 bytes
                            }
                            else
                            {
                                return 3; //16 bytes
                            }
                        } // End 12/16
                        
                    } // End 4/8/12/16
                    else
                    {
                        if (aObjectSize < 25)
                        {
                            if (aObjectSize < 21)
                            {
                                return 4; //20 bytes
                            }
                            else
                            {
                                return 5; //24 bytes
                            }
                        } //End 20/24
                        else
                        {
                            if (aObjectSize < 29)
                            {
                                return 6; //28 bytes
                            }
                            else
                            {
                                return 7; //32 bytes
                            }
                        } //End 28/32
                    } //End 20/24/28/32
                } //End 4/8/12/16/20/24/28/32
                else
                {
                    if (aObjectSize < 129)
                    {
                        if (aObjectSize < 65)
                        {
                            if (aObjectSize < 49)
                            {
                                return 8; // 48 bytes
                            }
                            else
                            {
                                return 9; // 64 bytes
                            }
                        }
                        else
                        {
                            if (aObjectSize < 97)
                            {
                                return 10; // 96 bytes
                            }
                            else
                            {
                                return 11; // 128 bytes
                            }
                        }
                    } //End 48/64/96/128
                    else
                    {
                        if (aObjectSize < 513)
                        {
                            if (aObjectSize < 257)
                            {
                                return 12; // 256 bytes
                            }
                            else
                            {
                                return 13; // 512 bytes
                            }
                        } //End 256/512
                        else
                        {
                            if (aObjectSize < 1025)
                            {
                                return 14; // 1024
                            }
                            else
                            {
                                return 15; // 2048
                            }
                        } //End 1024/2048
                    } // End 256/512/1024/2048
                } //End 48/64/96/128//256/512/1024/2048
                return -1;
            }

            inline static UInt32 ConvertSizeToPoolSize(UInt32 aObjectSize)
            {
                if (aObjectSize == 0)
                {
                    return 0;
                }
                //Split Half
                if (aObjectSize < 33)
                {
                    if (aObjectSize < 17)
                    {
                        if (aObjectSize < 9)
                        {
                            if (aObjectSize < 5)
                            {
                                return 4; //4 bytes
                            }
                            else
                            {
                                return 8; //8 bytes;
                            }
                        } // End 4/8
                        else
                        {
                            if (aObjectSize < 13)
                            {
                                return 12; //12 bytes
                            }
                            else
                            {
                                return 16; //16 bytes
                            }
                        } // End 12/16

                    } // End 4/8/12/16
                    else
                    {
                        if (aObjectSize < 25)
                        {
                            if (aObjectSize < 21)
                            {
                                return 20; //20 bytes
                            }
                            else
                            {
                                return 24; //24 bytes
                            }
                        } //End 20/24
                        else
                        {
                            if (aObjectSize < 29)
                            {
                                return 28; //28 bytes
                            }
                            else
                            {
                                return 32; //32 bytes
                            }
                        } //End 28/32
                    } //End 20/24/28/32
                } //End 4/8/12/16/20/24/28/32
                else
                {
                    if (aObjectSize < 129)
                    {
                        if (aObjectSize < 65)
                        {
                            if (aObjectSize < 49)
                            {
                                return 48; // 48 bytes
                            }
                            else
                            {
                                return 64; // 64 bytes
                            }
                        }
                        else
                        {
                            if (aObjectSize < 97)
                            {
                                return 96; // 96 bytes
                            }
                            else
                            {
                                return 128; // 128 bytes
                            }
                        }
                    } //End 48/64/96/128
                    else
                    {
                        if (aObjectSize < 513)
                        {
                            if (aObjectSize < 257)
                            {
                                return 256; // 256 bytes
                            }
                            else
                            {
                                return 512; // 512 bytes
                            }
                        } //End 256/512
                        else
                        {
                            if (aObjectSize < 1025)
                            {
                                return 1024; // 1024
                            }
                            else
                            {
                                return 2048; // 2048
                            }
                        } //End 1024/2048
                    } // End 256/512/1024/2048
                } //End 48/64/96/128//256/512/1024/2048
                return 0;
            }


			inline static void GetCorrectObjectInfo(UInt32 aObjectSize, UInt32 & aCorrectSize, SInt8 & aIndex)
			{
				aCorrectSize = 0;
				aIndex = -1;

				if (aObjectSize == 0)
				{
					aCorrectSize = 0;
					aIndex = -1;
					return;
				}
				//Split Half
				if (aObjectSize < 33)
				{
					if (aObjectSize < 17)
					{
						if (aObjectSize < 9)
						{
							if (aObjectSize < 5)
							{
								aIndex = 0;
								aCorrectSize = 4;
							}
							else
							{
								aIndex = 1;
								aCorrectSize = 8;
							}
						} // End 4/8
						else
						{
							if (aObjectSize < 13)
							{
								aIndex = 2;
								aCorrectSize = 12;
							}
							else
							{
								aIndex = 3;
								aCorrectSize = 16;
							}
						} // End 12/16

					} // End 4/8/12/16
					else
					{
						if (aObjectSize < 25)
						{
							if (aObjectSize < 21)
							{
								aIndex = 4;
								aCorrectSize = 20;
							}
							else
							{
								aIndex = 5;
								aCorrectSize = 24;
							}
						} //End 20/24
						else
						{
							if (aObjectSize < 29)
							{
								aIndex = 6;
								aCorrectSize = 28;
							}
							else
							{
								aIndex = 7;
								aCorrectSize = 32;
							}
						} //End 28/32
					} //End 20/24/28/32
				} //End 4/8/12/16/20/24/28/32
				else if (aObjectSize < 2049)
				{
					if (aObjectSize < 129)
					{
						if (aObjectSize < 65)
						{
							if (aObjectSize < 49)
							{
								aIndex = 8;
								aCorrectSize = 48;
							}
							else
							{
								aIndex = 9;
								aCorrectSize = 64;
							}
						}
						else
						{
							if (aObjectSize < 97)
							{
								aIndex = 10;
								aCorrectSize = 96;
							}
							else
							{
								aIndex = 11;
								aCorrectSize = 128;
							}
						}
					} //End 48/64/96/128
					else
					{
						if (aObjectSize < 513)
						{
							if (aObjectSize < 257)
							{
								aIndex = 12;
								aCorrectSize = 256;
							}
							else
							{
								aIndex = 13;
								aCorrectSize = 512;
							}
						} //End 256/512
						else
						{
							if (aObjectSize < 1025)
							{
								aIndex = 14;
								aCorrectSize = 1024;
							}
							else
							{
								aIndex = 15;
								aCorrectSize = 2048;
							}
						} //End 1024/2048
					} // End 256/512/1024/2048
				} //End 48/64/96/128//256/512/1024/2048
				

			}

			inline static AllocatorType GetAllocatorType(void * aAddress)
			{
				MemoryHeader * header = (MemoryHeader*)SubtractPtr(aAddress, sizeof(MemoryHeader));
				UInt8 flags;
				UInt8 id;
				UInt16 size;
			
				header->Read(flags, id, size);
			
				if ((flags & MemoryFlags::POOL) == MemoryFlags::POOL)
				{
					return AllocatorType::Pool;
				}
				else if ((flags & MemoryFlags::STACK) == MemoryFlags::STACK)
				{
					return AllocatorType::Stack;
				}
				else if ((flags & MemoryFlags::FRAME) == MemoryFlags::FRAME)
				{
					return AllocatorType::Frame;
				}
			
				return AllocatorType::Pool;
			}

        };


		
    }
			
}

#endif