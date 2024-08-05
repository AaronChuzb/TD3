idf_component_register(
  SRC_DIRS
  "."
  "HAL"
  "Model"
  "View"
  "View/Fonts"
  "View/Assets"
  "View/Pages/StartUp"
  "View/Pages/Auth"
  "View/Pages/Home"
  "View/Pages/Clock"
  "View/Pages/Pressure"
  "View/Pages/Setting"
  "View/Pages/Gyro"
  "View/Pages/Files"
  "View/Pages/Battery"
  "View/Pages/Weather"
  "View/Pages/Network"
  "View/Pages/Terminal"
          
  INCLUDE_DIRS 
  "."
  "HAL"
  "Model"
  "View"
  "View/Fonts"
  "View/Assets"
  "View/Pages/StartUp"
  "View/Pages/Auth"
  "View/Pages/Home"
  "View/Pages/Clock"
  "View/Pages/Pressure"
  "View/Pages/Setting"
  "View/Pages/Gyro"
  "View/Pages/Files"
  "View/Pages/Battery"
  "View/Pages/Weather"
  "View/Pages/Network"
  "View/Pages/Terminal"

  EMBED_TXTFILES ./Resource/root_cert.pem
)
fatfs_create_spiflash_image(storage ../storage FLASH_IN_PROJECT)
