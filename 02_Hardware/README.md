# Hardware del Proyecto

🔧 Hardware del Proyecto – Chaleco Inteligente

Descripción General

Esta sección documenta la arquitectura física y electrónica del sistema, incluyendo la selección de componentes, diseño energético, integración de sensores y estructura mecánica del chaleco inteligente.

El hardware fue diseñado considerando:

Bajo consumo energético

Peso reducido

Portabilidad

Estabilidad eléctrica

Integración modular

🏗 Arquitectura del Sistema

El sistema está compuesto por los siguientes subsistemas:

🔹 1. Unidad de Procesamiento

ESP32-WROOM-32E

Control general del sistema

Gestión de comunicación I2C y UART

Procesamiento de datos fisiológicos y biomecánicos

🔹 2. Sensores Fisiológicos

MAX30102 – Frecuencia cardiaca y SpO₂

Medición óptica por fotopletismografía

Integración vía I2C

🔹 3. Sensores de Movimiento

BNO055 (IMU 9 ejes)

Acelerómetro

Giroscopio

Magnetómetro

Cálculo de orientación absoluta

🔹 4. Sensores Ambientales

BMP280

Presión barométrica

Temperatura

Estimación indirecta de altitud

🔹 5. Geolocalización

GPS NEO-6MV2

Posición

Velocidad

Tiempo UTC

🔹 6. Comunicación

SIM800L (GSM/GPRS)

Transmisión remota de datos

Envío de alertas (si aplica)

🔹 7. Sistema de Alimentación

Batería Li-ion 18650 (3.7V – 3000mAh)

TP4056 (módulo de carga)

MT3608 (convertidor Step-Up)

Diseñado para garantizar estabilidad ante picos de consumo, especialmente en transmisión GSM.

📂 Contenido de esta Carpeta

📄 Datasheets oficiales de cada componente.

Archivos de diseño electrónico (esquemáticos y PCB).

Modelos mecánicos (STL / CAD).

 Diagramas de arquitectura física.

🔌 Diagrama de conexiones del sistema.

 Consideraciones de Diseño

Durante el desarrollo del hardware se priorizó:

Minimizar interferencias eléctricas.

Reducir peso total del sistema.

Optimizar distribución de sensores en el chaleco.

Garantizar estabilidad de alimentación.

Facilitar mantenimiento y reemplazo modular.

🧪 Estado Actual del Hardware

Actualmente el sistema se encuentra en fase de:

Integración progresiva de módulos.

Pruebas unitarias por sensor.

Validación de consumo energético.

Optimización de estabilidad eléctrica.
