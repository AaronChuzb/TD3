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
)
fatfs_create_spiflash_image(storage ../storage FLASH_IN_PROJECT)