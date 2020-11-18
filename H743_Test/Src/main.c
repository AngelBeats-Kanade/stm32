/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

DMA2D_HandleTypeDef hdma2d;

LTDC_HandleTypeDef hltdc;

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */
typedef struct
{
  uint32_t TextColor;
  uint32_t BackColor;
  sFONT *pFont;
} LCD_DrawPropTypeDef;

typedef struct
{
  int16_t X;
  int16_t Y;
} Point, *pPoint;

typedef enum
{
  CENTER_MODE = 0X01,
  RIGHT_MODE = 0X02,
  LEFT_MODE = 0X03
} Text_AlignModeTypedef;

static LCD_DrawPropTypeDef DrawProp[2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_LTDC_Init(void);
/* USER CODE BEGIN PFP */
void LCD_SetLayerAddress(uint16_t LayerIndex, uint32_t Address);
void LCD_SetTextColor(uint16_t LayerIndex, uint32_t Color);
void LCD_SetBackColor(uint16_t LayerIndex, uint32_t Color);
void LCD_SetColors(uint16_t LayerIndex, uint32_t TextColor, uint32_t BackColor);
void LCD_SetFont(uint16_t LayerIndex, sFONT *fonts);
void LCD_DrawPixel(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code);
void LCD_DisplayChar(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine,
                          uint32_t ColorIndex);
static void DrawChar(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16_t LayerIndex, uint32_t Color);
uint32_t LCD_GetXSize(uint16_t LayerIndex);
uint32_t LCD_GetYSize(uint16_t LayerIndex);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  HAL_MspInit();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  Board_MPU_Config(0, MPU_Normal_WT, 0xD0000000, MPU_32MB);
  Board_MPU_Config(1, MPU_Normal_WT, 0x24000000, MPU_512KB);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_LTDC_Init();
  /* USER CODE BEGIN 2 */
  LCD_Clear(0, LCD_COLOR_BLACK);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
    HAL_Delay(500);
    LCD_SetColors(0, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
    LCD_SetFont(0, &Font24);
    LCD_DisplayChar(0, 30, 30, 0x24);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable
   */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }
  /** Macro to configure the PLL clock source
   */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the CPU, AHB and APB busses clocks
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_FMC;
  PeriphClkInitStruct.PLL2.PLL2M = 5;
  PeriphClkInitStruct.PLL2.PLL2N = 144;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 3;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 270;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 10;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief DMA2D Initialization Function
 * @param None
 * @retval None
 */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_R2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.Init.BytesSwap = DMA2D_BYTES_REGULAR;
  hdma2d.Init.LineOffsetMode = DMA2D_LOM_PIXELS;
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */
  HAL_DMA2D_MspInit(&hdma2d);
  /* USER CODE END DMA2D_Init 2 */
}

/**
 * @brief LTDC Initialization Function
 * @param None
 * @retval None
 */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 0;
  hltdc.Init.VerticalSync = 0;
  hltdc.Init.AccumulatedHBP = 46;
  hltdc.Init.AccumulatedVBP = 23;
  hltdc.Init.AccumulatedActiveW = 846;
  hltdc.Init.AccumulatedActiveH = 503;
  hltdc.Init.TotalWidth = 856;
  hltdc.Init.TotalHeigh = 525;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 800;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 480;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0xd2000000;
  pLayerCfg.ImageWidth = 800;
  pLayerCfg.ImageHeight = 480;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 800;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 480;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg1.Alpha = 255;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg1.FBStartAdress = 0xD2000000 + 800 * 480 * 4;
  pLayerCfg1.ImageWidth = 800;
  pLayerCfg1.ImageHeight = 480;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */
  DrawProp[1].BackColor = LCD_COLOR_WHITE;
  DrawProp[1].pFont = &LCD_DEFAULT_FONT;
  DrawProp[1].TextColor = LCD_COLOR_BLACK;
  __HAL_LTDC_RELOAD_CONFIG(&hltdc);

  DrawProp[0].BackColor = LCD_COLOR_WHITE;
  DrawProp[0].pFont = &LCD_DEFAULT_FONT;
  DrawProp[0].TextColor = LCD_COLOR_BLACK;
  __HAL_LTDC_RELOAD_CONFIG(&hltdc);

  HAL_LTDC_MspInit(&hltdc);
  /* USER CODE END LTDC_Init 2 */
}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
   */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 8;
  SdramTiming.SelfRefreshTime = 5;
  SdramTiming.RowCycleDelay = 8;
  SdramTiming.WriteRecoveryTime = 4;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN FMC_Init 2 */
  HAL_SDRAM_MspInit(&hsdram1);
  /* USER CODE END FMC_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LTDC_BL_GPIO_Port, LTDC_BL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LTDC_BL_Pin */
  GPIO_InitStruct.Pin = LTDC_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LTDC_BL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_R_Pin */
  GPIO_InitStruct.Pin = LED_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_R_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
void LCD_SetLayerAddress(uint16_t LayerIndex, uint32_t Address) { HAL_LTDC_SetAddress(&hltdc, Address, LayerIndex); }

void LCD_SetTextColor(uint16_t LayerIndex, uint32_t Color) { DrawProp[LayerIndex].TextColor = Color; }

void LCD_SetBackColor(uint16_t LayerIndex, uint32_t Color) { DrawProp[LayerIndex].BackColor = Color; }

void LCD_SetFont(uint16_t LayerIndex, sFONT *fonts) { DrawProp[LayerIndex].pFont = fonts; }

void LCD_SetColors(uint16_t LayerIndex, uint32_t TextColor, uint32_t BackColor)
{
  LCD_SetTextColor(LayerIndex, TextColor);
  LCD_SetBackColor(LayerIndex, BackColor);
}

/**
 * @brief  ???LCD?????§¹??X????§³
 * @param  LayrerIndex??????????
 * @retval X????§³
 */
uint32_t LCD_GetXSize(uint16_t LayerIndex) { return hltdc.LayerCfg[LayerIndex].ImageWidth; }

/**
 * @brief  ???LCD?????§¹??Y????§³
 * @retval Y????§³
 */
uint32_t LCD_GetYSize(uint16_t LayerIndex) { return hltdc.LayerCfg[LayerIndex].ImageHeight; }

/**
 * @brief  ?????????
 * @param  LayrerIndex??????????
 * @param  Xpos: X?????????
 * @param  Ypos: Y?????????
 * @param  Ascii: ??? ascii ??,??¦¶?? 0x20 ??0x7E ??
 * @retval ??
 */
void LCD_DisplayChar(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  DrawChar(LayerIndex, Xpos, Ypos,
           &DrawProp[LayerIndex].pFont->table[(Ascii - ' ') * DrawProp[LayerIndex].pFont->Height *
                                              ((DrawProp[LayerIndex].pFont->Width + 7) / 8)]);
}

/**
 * @brief ?????????
 * @param LayerIndex???????????
 * @param Xpos????????????¦Ë??
 * @param Ypos????????????¦Ë??
 * @param c?????????????????
 * @retval ??
 */
static void DrawChar(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t offset;
  uint8_t *pchar;
  uint32_t line;

  height = DrawProp[LayerIndex].pFont->Height; //????????????????
  width = DrawProp[LayerIndex].pFont->Width;   //????????????????

  offset = 8 * ((width + 7) / 8) - width; //???????????????????????????›¥??§³-??????

  for (i = 0; i < height; i++) //????????????
  {
    pchar = ((uint8_t *)c + (width + 7) / 8 * i); //???????????????????????

    switch (((width + 7) / 8))
    {
    case 1:
      line = pchar[0]; //?????????§³??8????????????
      break;

    case 2:
      line = (pchar[0] << 8) | pchar[1]; //????????????8§³??16????????????
      break;

    case 3:
    default:
      line = (pchar[0] << 16) | (pchar[1] << 8) | pchar[2]; //????????????16§³??24????????????
      break;
    }

    for (j = 0; j < width; j++) //????????????
    {
      if (line & (1 << (width - j + offset - 1))) //???????§Ö???????????¦Ë?????????????????§Ý??
      {
        LCD_DrawPixel(LayerIndex, (Xpos + j), Ypos, DrawProp[LayerIndex].TextColor);
      }
      else //????????????????????????????????
      {
        LCD_DrawPixel(LayerIndex, (Xpos + j), Ypos, DrawProp[LayerIndex].BackColor);
      }
    }
    Ypos++;
  }
}

/**
 * @brief ????????????
 * @param Xpos??X??????
 * @param Ypos??Y??????
 * @param RGB_Code??????????
 * @param LayerIndex???????????
 * @retval ??
 */
void LCD_DrawPixel(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{

  if (hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    *(__IO uint32_t *)((hltdc.LayerCfg[LayerIndex].FBStartAdress) + (4 * (Ypos * LCD_GetXSize(LayerIndex) + Xpos))) =
        RGB_Code;
  }
  else if (hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    *(__IO uint8_t *)(hltdc.LayerCfg[LayerIndex].FBStartAdress + (3 * (Ypos * LCD_GetXSize(LayerIndex) + Xpos)) + 2) =
        0xFF & (RGB_Code >> 16);
    *(__IO uint8_t *)(hltdc.LayerCfg[LayerIndex].FBStartAdress + (3 * (Ypos * LCD_GetXSize(LayerIndex) + Xpos)) + 1) =
        0xFF & (RGB_Code >> 8);
    *(__IO uint8_t *)(hltdc.LayerCfg[LayerIndex].FBStartAdress + (3 * (Ypos * LCD_GetXSize(LayerIndex) + Xpos))) =
        0xFF & RGB_Code;
  }
  else if ((hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) ||
           (hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) ||
           (hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_AL88))
  {
    *(__IO uint16_t *)(hltdc.LayerCfg[LayerIndex].FBStartAdress + (2 * (Ypos * LCD_GetXSize(LayerIndex) + Xpos))) =
        (uint16_t)RGB_Code;
  }
  else
  {
    *(__IO uint16_t *)(hltdc.LayerCfg[LayerIndex].FBStartAdress + (2 * (Ypos * LCD_GetXSize(LayerIndex) + Xpos))) =
        (uint16_t)RGB_Code;
  }
}

/**
 * @brief  ??????
 * @retval ??
 */
void LCD_DisplayOn(void)
{
  /* ????? */
  __HAL_LTDC_ENABLE(&hltdc);
  HAL_GPIO_WritePin(LTDC_BL_GPIO_Port, LTDC_BL_Pin, GPIO_PIN_SET); /* ??????*/
}

/**
 * @brief  ???????
 * @retval ??
 */
void LCD_DisplayOff(void)
{
  /* ????? */
  __HAL_LTDC_DISABLE(&hltdc);
  HAL_GPIO_WritePin(LTDC_BL_GPIO_Port, LTDC_BL_Pin, GPIO_PIN_RESET); /*?????*/
}

/**
 * @brief  LCD?????????
 * @param  Color: ???????
 * @retval None
 */
void LCD_Clear(uint16_t LayerIndex, uint32_t Color)
{
  /* ???? */
  LL_FillBuffer(LayerIndex, (uint32_t *)(hltdc.LayerCfg[LayerIndex].FBStartAdress), LCD_GetXSize(LayerIndex),
                LCD_GetYSize(LayerIndex), 0, Color);
}

/**
 * @brief  ????????????
 * @param  LayerIndex: ?????
 * @param  pDst: ?????????????
 * @param  xSize: ?????????
 * @param  ySize: ?????????
 * @param  OffLine: ?????
 * @param  ColorIndex: ??????
 * @retval None
 */
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine,
                          uint32_t ColorIndex)
{
  hdma2d.Init.Mode = DMA2D_R2M;
  if (hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
  {
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
  }
  else if (hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  }
  else if (hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB888;
  }
  else if (hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555)
  {
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB1555;
  }
  else if (hltdc.LayerCfg[LayerIndex].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444)
  {
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB4444;
  }
  hdma2d.Init.OutputOffset = OffLine;

  hdma2d.Instance = DMA2D;

  /* DMA2D ????? */
  if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
  {
    if (HAL_DMA2D_ConfigLayer(&hdma2d, LayerIndex) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* DMA??????? */
        HAL_DMA2D_PollForTransfer(&hdma2d, 10);
      }
    }
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
