/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../../inc/MarlinConfig.h"

#include <SPI.h>

#define IS_SPI(N) (BOARD_NR_SPI >= N && (TFT_SCK_PIN == BOARD_SPI##N##_SCK_PIN) && (TFT_MOSI_PIN == BOARD_SPI##N##_MOSI_PIN) && (TFT_MISO_PIN == BOARD_SPI##N##_MISO_PIN))
#if IS_SPI(1)
  #define TFT_SPI_DEVICE  1
  #define SPIdev          SPI1
  #define DMAx            DMA1
  #define DMA_CHx         DMA_CH3
#elif IS_SPI(2)
  #define TFT_SPI_DEVICE  2
  #define SPIdev          SPI2
  #define DMAx            DMA1
  #define DMA_CHx         DMA_CH5
#elif IS_SPI(3)
  #define TFT_SPI_DEVICE  3
  #define SPIdev          SPI3
  #define DMAx            DMA2
  #define DMA_CHx         DMA_CH2
#else
  #error "Invalid TFT SPI configuration."
#endif
#undef IS_SPI

#ifndef LCD_READ_ID
  #define LCD_READ_ID 0x04   // Read display identification information (0xD3 on ILI9341)
#endif
#ifndef LCD_READ_ID4
  #define LCD_READ_ID4 0xD3   // Read display identification information (0xD3 on ILI9341)
#endif

#define DATASIZE_8BIT  DATA_SIZE_8BIT
#define DATASIZE_16BIT DATA_SIZE_16BIT
#define TFT_IO_DRIVER  TFT_SPI
<<<<<<< HEAD
#define DMA_MAX_SIZE   0xFFFF
=======
#define DMA_MAX_WORDS  0xFFFF
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

#define DMA_MINC_ENABLE   DMA_MINC_MODE
#define DMA_MINC_DISABLE  0

class TFT_SPI {
private:
<<<<<<< HEAD
  static uint32_t ReadID(uint16_t Reg);
  static void Transmit(uint16_t Data);
  static void Transmit(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);
  static void TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);
=======
  static uint32_t readID(const uint16_t inReg);
  static void transmit(uint16_t data);
  static void transmit(uint32_t memoryIncrease, uint16_t *data, uint16_t count);
  static void transmitDMA(uint32_t memoryIncrease, uint16_t *data, uint16_t count);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

public:
  static SPIClass SPIx;

<<<<<<< HEAD
  static void Init();
  static uint32_t GetID();
  static bool isBusy();
  static void Abort();

  static void DataTransferBegin(uint16_t DataWidth=DATA_SIZE_16BIT);
  static void DataTransferEnd() { WRITE(TFT_CS_PIN, HIGH); SPIx.end(); };
  static void DataTransferAbort();

  static void WriteData(uint16_t Data) { Transmit(Data); }
  static void WriteReg(uint16_t Reg) { WRITE(TFT_DC_PIN, LOW); Transmit(Reg); WRITE(TFT_DC_PIN, HIGH); }

  static void WriteSequence_DMA(uint16_t *Data, uint16_t Count) { TransmitDMA(DMA_MINC_ENABLE, Data, Count); }
  static void WriteMultiple_DMA(uint16_t Color, uint16_t Count) { static uint16_t Data; Data = Color; TransmitDMA(DMA_MINC_DISABLE, &Data, Count); }

  static void WriteSequence(uint16_t *Data, uint16_t Count) { Transmit(DMA_MINC_ENABLE, Data, Count); }
  static void WriteMultiple(uint16_t Color, uint32_t Count) {
    while (Count > 0) {
      Transmit(DMA_MINC_DISABLE, &Color, Count > DMA_MAX_SIZE ? DMA_MAX_SIZE : Count);
      Count = Count > DMA_MAX_SIZE ? Count - DMA_MAX_SIZE : 0;
=======
  static void init();
  static uint32_t getID();
  static bool isBusy();
  static void abort();

  static void dataTransferBegin(uint16_t dataWidth=DATA_SIZE_16BIT);
  static void dataTransferEnd() { WRITE(TFT_CS_PIN, HIGH); SPIx.end(); };
  static void dataTransferAbort();

  static void writeData(uint16_t data) { transmit(data); }
  static void writeReg(const uint16_t inReg) { WRITE(TFT_DC_PIN, LOW); transmit(inReg); WRITE(TFT_DC_PIN, HIGH); }

  static void writeSequence_DMA(uint16_t *data, uint16_t count) { transmitDMA(DMA_MINC_ENABLE, data, count); }
  static void writeMultiple_DMA(uint16_t color, uint16_t count) { static uint16_t data; data = color; transmitDMA(DMA_MINC_DISABLE, &data, count); }

  static void writeSequence(uint16_t *data, uint16_t count) { transmit(DMA_MINC_ENABLE, data, count); }
  static void writeMultiple(uint16_t color, uint32_t count) {
    while (count > 0) {
      transmit(DMA_MINC_DISABLE, &color, count > DMA_MAX_WORDS ? DMA_MAX_WORDS : count);
      count = count > DMA_MAX_WORDS ? count - DMA_MAX_WORDS : 0;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  }
};
