├── Drivers/
│   ├── spi.c/h              ← SPI驱动（寄存器+CMSIS）
│   ├── oled.c/h     ← OLED SSD1306驱动
│   ├── gpio.c/h             ← GPIO配置
│   └── clock.c/h            ← 时钟系统初始化
├── Core/
│   ├── main.c               ← 主程序
│   ├── system.c/h           ← 系统初始化
│   └── interrupt.c          ← 中断处理
├── Game/
│   ├── tetris.c/h           ← 游戏核心逻辑
│   ├── graphics.c/h         ← 图形渲染
│   └── input.c/h            ← 按键输入
├── startup_stm32f103c8.s    ← 汇编启动文件
└── linker_stm32f103c8.ld    ← 链接脚本
