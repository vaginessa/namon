/** 
 *  @file       fileHandler.hpp
 *  @brief      File handler header file
 *  @author     Jozef Zuzelka (xzuzel00)
 *  Mail:       xzuzel00@stud.fit.vutbr.cz
 *  @date
 *  - Created:  06.03.2017 14:50
 *  - Edited:       22.03.2017 17:10
 *  Version:    1.0.0
 */

#pragma once

#include <fstream>              //  ofstream



/*!
 * @brief       Creates the output file and writes SectionHeaderBlock and InterfaceDescriptionBlock to the file
 * @param[in]   oFile   The output file
 */
void initOFile(std::ofstream & oFile);
