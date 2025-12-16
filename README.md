# A Real-Time IoT System for Hyper-Local Noise Source Identification using LoRa

Este repositório contém o código fonte e a documentação para um sistema de monitoramento de ruído urbano em tempo real, de baixo custo e alta eficiência energética. O projeto utiliza a tecnologia **LoRa** para comunicação de longo alcance e uma arquitetura IoT modular para identificar fontes de ruído de forma hiperlocal.

Desenvolvido no **Centro de Informática (CIn) - UFPE**.

## 📖 Visão Geral

A poluição sonora urbana é um problema crítico de saúde pública. Este projeto visa solucionar a falta de monitoramento contínuo e escalável através de uma rede de sensores que:

1.  **Captura** níveis de som ambiente (dB) continuamente.
2.  **Processa** os dados localmente para garantir eficiência energética.
3.  **Transmite** informações via LoRa para um gateway central.
4.  **Visualiza** os dados em um dashboard web em tempo real.

### Principais Características
* **Comunicação LoRa:** Utiliza topologia estrela (single-hop) para transmissão de dados sem necessidade de relés intermediários.
* **Eficiência Energética:** O nó sensor implementa um mecanismo de decisão local, transmitindo dados apenas quando a variação do ruído excede um limite pré-definido, reduzindo o tráfego na rede e o consumo de energia.
* **Baixo Custo:** Utiliza componentes acessíveis (ESP32 e microfones analógicos) em comparação a sensores industriais.
* **Monitoramento Hiperlocal:** Capaz de identificar distúrbios de ruído específicos (ex: um andar específico de um prédio).

## Hardware

O sistema é composto por duas unidades principais: o **Nó Sensor** e o **Gateway**.

| Componente | Especificação | Função |
| :--- | :--- | :--- |
| **Microcontrolador** | [Heltec WiFi LoRa 32 V2](https://heltec.org/) | Processamento e comunicação (LoRa/Wi-Fi). Usado tanto nos Nós quanto no Gateway. |
| **Sensor de Som** | MAX4466 | Módulo de microfone eletreto com ganho ajustável para captura do nível de pressão sonora. |

## Arquitetura de Software

* **Firmware (Embedded):** Desenvolvido em **C++** utilizando o framework **PlatformIO**.
* **Backend:** **Firebase Realtime Database** (NoSQL) para armazenamento e sincronização de dados na nuvem.
* **Frontend:** Dashboard web desenvolvido com **HTML5** e **Chart.js** para renderização de gráficos de linha em tempo real.

## 🚀 Como Executar

### Pré-requisitos
* [Visual Studio Code](https://code.visualstudio.com/)
* Extensão [PlatformIO IDE](https://platformio.org/)
* Conta no Google Firebase

### Passos de Instalação

1.  **Clonar o Repositório**
    ```bash
    git clone [https://github.com/ligiaferro/LiVi.git](https://github.com/ligiaferro/LiVi.git)
    ```

2.  **Configuração do Firmware**
    * Abra a pasta do projeto no PlatformIO.
    * **Nó Sensor:** Ajuste os parâmetros de sensibilidade do microfone e o intervalo de leitura no `main.cpp`.
    * **Gateway:** Configure as credenciais Wi-Fi (SSID/Password) e as chaves de API do Firebase.

3.  **Hardware**
    * Conecte o pino `OUT` do MAX4466 a um pino ADC do ESP32 (conforme esquemático).
    * Alimente as placas via USB ou bateria Li-Po.

4.  **Dashboard**
    * Abra o `index.html` localmente.
    * Certifique-se de que o script de configuração do Firebase corresponde ao seu projeto.

## 👥 Autores

* **Lígia Ferro Braga Laurindo de Cerqueira Padilha** - *UFPE-CIN* - [lfblcp@cin.ufpe.br](mailto:lfblcp@cin.ufpe.br)
* **Victória Xavier Queiroz** - *UFPE-CIN* - [vxq@cin.ufpe.br](mailto:vxq@cin.ufpe.br)

## 📄 Licença

Este projeto é parte de uma iniciativa de pesquisa acadêmica.

---
