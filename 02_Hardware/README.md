# Hardware del Proyecto
🔧 Hardware del Proyecto – Chaleco Inteligente
Descripción

Esta sección documenta la arquitectura electrónica y física del sistema, incluyendo selección de componentes, diseño energético e integración de sensores.

El hardware fue diseñado bajo criterios de:

Bajo consumo energético

Peso reducido

Integración modular

Estabilidad eléctrica

🏗 Arquitectura del Sistema

Unidad de procesamiento

ESP32-WROOM-32E

Sensores fisiológicos

MAX30102 (Frecuencia cardiaca y SpO₂)

Sensores de movimiento

BNO055 (IMU 9 ejes)

Sensores ambientales

BMP280 (Presión y temperatura)

Geolocalización

GPS NEO-6MV2

Comunicación

SIM800L (GSM/GPRS)

Sistema de alimentación

Batería Li-ion 18650 (3000mAh)

TP4056 (carga)

MT3608 (Step-Up)

📂 Contenido de esta Carpeta

Datasheets oficiales

Esquemáticos y diseño PCB

Modelos mecánicos (STL / CAD)

Diagramas de arquitectura

Diagrama de conexiones

⚡ Enfoque de Diseño

Integración progresiva por módulos

Validación individual antes de integración total

Control de consumo energético

Optimización de estabilidad eléctrica
