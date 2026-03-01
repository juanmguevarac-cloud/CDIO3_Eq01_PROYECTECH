💻 Firmware – Sistema Embebido
Descripción

Esta carpeta contiene el código del sistema embebido encargado de la adquisición, procesamiento y gestión de los datos fisiológicos y biomecánicos del chaleco inteligente.

El firmware fue desarrollado para el ESP32-WROOM-32E, integrando múltiples sensores y módulos de comunicación bajo una arquitectura modular.

⚙️ Funcionalidades Principales

Lectura de sensores (MAX30102, BNO055, BMP280, GPS).

Procesamiento básico de señales.

Gestión de comunicación (I2C / UART).

Control del sistema de alimentación.

Transmisión y/o almacenamiento de datos.

📂 Contenido de la Carpeta

Código fuente principal.

Módulos por sensor.

Librerías externas utilizadas.

Archivos de configuración.

Instrucciones de compilación y carga.

🔧 Enfoque de Desarrollo

Arquitectura modular por subsistema.

Pruebas unitarias antes de integración completa.

Optimización de consumo energético.

Manejo de errores y estabilidad del sistema.
