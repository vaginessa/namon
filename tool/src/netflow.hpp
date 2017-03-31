/** 
 *  @file       netflow.hpp
 *  @brief      Netflow structure header file
 *  @author     Jozef Zuzelka <xzuzel00@stud.fit.vutbr.cz>
 *  @date
 *   - Created: 26.02.2017 23:13
 *   - Edited:  01.04.2017 00:01
 */

#pragma once

#include <string>           //  string
#include <fstream>          //  ostream
#include <netinet/in.h>     //  in_addr, in6_addr
#include <arpa/inet.h>      //  inet_ntop()
#include <cstring>          //  memcpy()

class TEntry;


/*!
 * @class Netflow
 * @brief Netflow class contains information about packet needed to uniquely determine
 * a netflow it belongs to.
 */
class Netflow
{
    uint8_t ipVersion       =0;         //!< IP header version
    /*!
     * @brief   Pointer to a structure containing local IP address
     * @details Type of the pointer is determined using #Netflow::ipVersion
     */
    void *localIp           = nullptr;
    uint16_t localPort      =0;         //!< Local port
    uint8_t proto           =0;         //!< Layer 4 protocol
    uint64_t startTime      =0;         //!< Time of the first packet which belongs to this netflow
    uint64_t endTime        =0;         //!< Time of the last packet which belongs to this netflow
public:
    /*!
     * @brief Defalut constructor
     */
    Netflow()                               {}
    /*!
     * @brief   Destructor cleans memory pointed by #Netflow::srcIp and #Netflow::dstIp pointers
     */
    ~Netflow();
    /*! 
     * @brief   Get method for #Netflow::ipVersion
     * @return  IP header version
     */
    uint8_t getIpVersion()                  { return ipVersion; }
    /*! 
     * @brief       Set method for #Netflow::ipVersion
     * @param[in]   ipV       IP header version
     */
    void setIpVersion(uint8_t ipV)          { ipVersion = ipV; }
    /*! 
     * @brief   Get method for #Netflow::localIp
     * @return  Pointer to local IP structure
     */
    void * getLocalIp()                     { return localIp; }
    /*! 
     * @brief       Set method for #Netflow::localIp
     * @pre         newIp must point to a valid in*_addr structure
     * @post        Memory pointed by newIp must exist as long as Netflow object exists.
     *              Then it will be freed in destructor.
     * @param[in]   newIp     Local IP structure pointer
     */
    void setLocalIp(void *newIp)            { localIp = newIp; }
    /*! 
     * @brief   Get method for #Netflow::localPort
     * @return  Local port
     */
    uint16_t getLocalPort()                 { return localPort; }
    /*! 
     * @brief       Set method for #Netflow::localPort
     * @param[in]   newPort   Local port
     */
    void setLocalPort(uint16_t newPort)     { localPort = newPort; }
    /*! 
     * @brief   Get method for #Netflow::proto
     * @return  Layer 4 protocol
     */
    uint8_t getProto()                      { return proto; }
    /*! 
     * @brief       Set method for #Netflow::proto
     * @param[in]   newProto  Layer 4 protocol
     */
    void setProto(uint8_t newProto)         { proto = newProto; }
    /*! 
     * @brief   Get method for #Netflow::startTime
     * @return  Time of the first packet which belongs to this netflow
     */
    uint64_t getStartTime()                 { return startTime; }
    /*! 
     * @brief       Set method for #Netflow::startTime
     * @param[in]   newTime   Time of the first packet which belongs to this netflow
     */
    void setStartTime(uint64_t newTime)     { startTime = newTime; }
    /*! 
     * @brief   Get method for #Netflow::endTime
     * @return  Time of the last packet which belongs to this netflow
     */
    uint64_t getEndTime()                   { return endTime; }
    /*! 
     * @brief       Set method for #Netflow::endTime
     * @param[in]   newTime   Time of the last packet which belongs to this netflow
     */
    void setEndTime(uint64_t newTime)       { endTime = newTime; }
    /*!
     * @brief   Function prints content of the Netflow structure
     */
    void print();
    /*!
     * @brief   Overloaded equality operator
     * @details Compares only netflow relevant variables
     */
    bool operator==(const Netflow& other) const;
    /*!
     * @brief   Copy assignment operator
     */
    Netflow& operator=(const Netflow& other)
    {
        if (this != &other)
        {
            if (ipVersion == 4)
            {
                if (localIp == nullptr)
                    localIp = new in_addr;
                memcpy(localIp, other.localIp, sizeof(in_addr));
            }
            else
            {
                if (localIp == nullptr)
                    localIp = new in6_addr;
                memcpy(localIp, other.localIp, sizeof(in6_addr));
            }
            
            ipVersion = other.ipVersion;
            localPort = other.localPort;
            proto = other.proto;
            startTime = other.startTime;
            endTime = other.endTime;
        }
        return *this;
    }
    /*!
     * @brief   Move assignment operator
     */
    Netflow& operator=(Netflow&& other)
    {
        if (this != &other)
        {
            if (ipVersion == 4)
                delete static_cast<in_addr*>(localIp);
            else
                delete static_cast<in6_addr*>(localIp);
            
            ipVersion = other.ipVersion;
            localIp = other.localIp;
            localPort = other.localPort;
            proto = other.proto;
            startTime = other.startTime;
            endTime = other.endTime;
            
            other.ipVersion = 0;
            other.localIp = nullptr;
            other.localPort = 0;
            other.proto = 0;
            other.startTime = 0;
            other.endTime = 0;
        }
        return *this;
    }
    /*!
     * @brief       Writes structure into the output file
     * @param[in]   file    The output file
     * @return      Amount of written data to the output file in bytes
     */
    unsigned int write(std::ofstream & file);
    friend class TEntry;
};