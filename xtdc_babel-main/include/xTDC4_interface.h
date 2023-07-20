//
// Header file containing structs and #defines specific for xTDC4 devices.
// The current driver version for xTDC4 devices is %VERSION%
//

/*! \file
 *  \brief The functions provided by the API are declared in
 *  xTDC4_interface.h.
 *
 *  The API is a DLL with C linkage.
 */
#ifndef XTDC4_INTERFACE_H
#define XTDC4_INTERFACE_H

#include "TDC4_interface.h"

#ifdef __linux__
#include <stdarg.h>
#include <stdint.h>
#endif

/*! \defgroup constants Constants
 */
/*! \defgroup initialization Initialization
 *@{
 */
/*! \defgroup initfuncts Functions for Initialization
 * @{
 */
/*! \defgroup defclose #defines for xtdc4_close()
 *
 *  xtdc4_close() returns one of the listed values, else
 *  - @link funcerrors XTDC4_OK @endlink
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*! \defgroup definit #defines for xtdc4_init()
 *
 *  xtdc4_init() returns one of the listed values, else
 *  - @link funcerrors XTDC4_OK @endlink
 */
/*! \defgroup defdefinpar #defines for xtdc4_get_default_init_parameters()
 *
 *  xtdc4_get_default_init_parameters() returns one of the listed values,
 *  else
 *  - @link funcerrors XTDC4_OK @endlink
 */
/*!@}*/
/*! \defgroup device Structure xtdc4_device
 */
/*! \defgroup initparamsstruct Structure xtdc4_init_parameters
 *  \brief struct for the initialization of the xTDC4
 *
 *  this structure MUST be completely INITIALIZED
 *  @{
 */
/*! \defgroup apiversion #define for version
 */
/*! \defgroup buffertype #defines for buffer_type
 *  \brief type of buffer
 */
/*! \defgroup devicetype #define for device_type
 */
/*!@}*/
/*!@}*/
/*! \defgroup status Status Information
 *  @{
 */
/*! \defgroup statfuncts Functions for Information Retrieval
 *  \brief Functions for getting detailed information about the xTDC4 board.
 *
 *  The driver provides functions to retrieve detailed information on the
 *  type of board, it's configuration, settings and state. The information is
 *  split according to its scope and the computational requirements to query the
 *  information from the board.
 *@{
 */
/*! \defgroup defparaminfo #defines for xtdc4_get_param_info()
 *
 *  xtdc4_get_param_info() returns one of the listed values, else
 *  - @link funcerrors XTDC4_OK @endlink
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*! \defgroup defstatinfo #defines for xtdc4_get_static_info()
 *
 *  xtdc4_get_static_info() returns one of the listed values, else
 *  - @link funcerrors XTDC4_OK @endlink
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*! \defgroup deffastinfo #defines for xtdc4_get_fast_info()
 *
 *  xtdc4_get_fast_info() returns one of the listed values, else
 *  - @link funcerrors XTDC4_OK @endlink
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*!@}*/
/*! \defgroup staticinfo Structure xtdc4_static_info
 *  \brief Structure contains static information
 *
 *  This structure contains information about the board that does not change
 *  during run time. It is provided by the function @link statfuncts
 *  xtdc4_get_static_info() @endlink.
 */
/*! \defgroup paraminfo Structure xtdc4_param_info
 *  \brief contains configuration changes
 *
 *  Structure filled by @link statfuncts xtdc4_get_param_info() @endlink.
 *  This structure contains information that change indirectly due to
 *  configuration changes.
 */
/*! \defgroup fastinfo Structure xtdc4_fast_info
 *  \brief contains fast dynamic information
 *
 *  This call returns a structure that contains dynamic information that can
 *  be obtained within a few microseconds.
 */
/*!@}*/
/*! \defgroup configuration Configuration
 *  \brief Configuration of xTDC4
 *  @{
 */
/*! \defgroup conffuncts Configuration Functions
 *  \brief Functions for the configuration of xTDC4
 *
 *  The device is configured with a configuration structure. The user should
 *  first obtain a structure that contains the default settings of the device
 *  read from an on board ROM, then modify the structure as needed for the user
 *  application and use the result to configure the device.
 * @{
 */
/*! \defgroup defconf #defines for xtdc4_configure()
 *
 *  xtdc4_configure() returns one of the listed values, else
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*! \defgroup defdefconf #defines for xtdc4_get_default_configuration()
 *
 *  xtdc4_get_default_configuration() returns one of the listed values, else
 *  - @link funcerrors XTDC4_OK @endlink
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*! \defgroup defcurconf #defines for xtdc4_get_current_configuration()
 *
 *  xtdc4_get_current_configuration() returns one of the listed values, else
 *  - @link funcerrors XTDC4_OK @endlink
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*!@}*/
/*! \defgroup confstruct Structure xtdc4_configuration
 *  \brief  contains configuration information
 *
 *  This structure contains the configuration information. It is used in
 *  conjunction with
 *  @link conffuncts xtdc4_get_default_configuration(),
 *  xtdc4_get_current_configuration() and xtdc4_configure() @endlink
 *
 *  Internally it uses the structures @link trigger xtdc4_trigger @endlink
 *  and @link tiger xtdc4_tiger_block @endlink
 * @{
 */
/*! \defgroup tdcmode #defines for tdc_mode
 *  \brief tdc_mode can be either grouped or continuous
 */
/*! \defgroup defdcoffset #defines for dc_offset
 *  \brief dc_offset values for various signal standards
 *
 *  used by @link xtdc4_configuration xtdc4_configuration @endlink.
 */
/*!@}*/
/*! \defgroup trigger Structure xtdc4_trigger
 *  \brief contains trigger settings
 */
/*! \defgroup tiger Structure xtdc4_tiger_block
 *  \brief contains settings of timing generator
 * @{
 */
/*! \defgroup deftriggersource #defines for sources
 *  \brief mask for choosing the trigger source
 */
/*!@}*/
/*! \defgroup channel Structure xtdc4_channel
 *  \brief contains TDC channel settings
 */
/*!@}*/
/*! \defgroup runtime Run Time Control
 *  \brief control functions during run time
 *  @{
 */
/*! \defgroup defstartcap #defines for xtdc4_start_capture()
 *
 *  xtdc4_start_capture() returns one of the listed values, else
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*! \defgroup defstopcap #defines for xtdc4_stop_capture()
 *
 *  xtdc4_stop_capture() returns one of the listed values, else
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*! \defgroup defpausecap #defines for xtdc4_pause_capture()
 *
 *  xtdc4_pause_capture() returns one of the listed values, else
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*! \defgroup defcontcap #defines for xtdc4_continue_capture()
 *
 *  xtdc4_continue_capture() returns one of the listed values, else
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*!@}*/
/*! \defgroup mainread Readout
 *  @{
 */
/*! \defgroup readout Functions for Readout
 *  \brief functions for data readout
 *  @{
 */
/*! \defgroup defack #defines for xtdc4_acknowledge()
 *
 *  xtdc4_acknowledge() returns one of the listed values, else
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*! \defgroup defread #defines for xtdc4_read()
 *
 *  xtdc4_read() returns one of the listed values, else
 *  - @link funcerrors XTDC4_INVALID_DEVICE @endlink
 */
/*!@}*/
/*! \defgroup readin Structure xtdc4_read_in
 *  \brief The parameters of the read commands
 */
/*! \defgroup strucreadout Structure xtdc4_read_out
 * @{
 */
/*! \defgroup deferror #defines for error_code
 *  \brief error code
 */
/*!@}*/
/*!@}*/
/*! \defgroup funcerrors Function Errors
 *
 *  All ERRORS must be positive, because the upper byte is used by
 *  crono_tools
 */
/*! \defgroup packflags Packet Flags
 *  \brief flags of the packet reporting error conditions
 */

// current version of the API
#define XTDC4_API_VERSION TDC4_API_VERSION

/*! \ingroup constants Constants
 * @{
 */
/*! \brief The number of TDC input channels.
 *
 *  used by
 *  - @link confstruct xtdc4_configuration::dc_offset @endlink
 *  - @link confstruct xtdc4_configuration::channel @endlink
 */
#define XTDC4_TDC_CHANNEL_COUNT 4
/*! \brief the number of digital input channels
 *
 *  used by
 * - @link confstruct xtdc4_configuration::lowres_channel @endlink
 */
#define XTDC4_LOWRES_CHANNEL_COUNT 3
/*! \brief The number of timing generators. One for each TDC input and one for
 *  the Start input.
 *
 *  used by
 *  - @link confstruct xtdc4_configuration::tiger_block[] @endlink
 */
#define XTDC4_TIGER_COUNT 5
/*! \brief The number of potential trigger sources for the timing generators.
 *
 *  One for each TDC input, one for the Start input plus some specials. Not
 *  all trigger sources are available on the xTDC4.
 *
 *  used by
 *  - @link confstruct xtdc4_configuration::trigger[] @endlink
 */
#define XTDC4_TRIGGER_COUNT 16 // including "config" for AUTO and ONE
/*!@}*/

/*! \ingroup buffertype
 *@{
 */
#define XTDC4_BUFFER_ALLOCATE 0 //!< either allocated (only option currently)
#define XTDC4_BUFFER_USE_PHYSICAL 1 //!< or physical
/*!@}*/

/*! \ingroup tdcmode
 *@{
 */
#define XTDC4_TDC_MODE_GROUPED 0    //!< grouped tdc_mode
#define XTDC4_TDC_MODE_CONTINUOUS 1 //!< continuous tdc_mode: not supported
/*!@}*/
/*! \defgroup defdefconf #defines for xtdc4_get_default_configuration()
 * default period of the auuto trigger: 200 kHz
 * base frequency 150 MHz
 */
#define XTDC4_DEFAULT_AUTO_TRIGGER_PERIOD 750
/*! \ingroup defdcoffset
 *@{
 */
// INPUT_BASELINE is 1.32V
#define XTDC4_DC_OFFSET_P_NIM +0.35
#define XTDC4_DC_OFFSET_P_CMOS +1.18
#define XTDC4_DC_OFFSET_P_LVCMOS_33 +1.18
#define XTDC4_DC_OFFSET_P_LVCMOS_25 +1.18
#define XTDC4_DC_OFFSET_P_LVCMOS_18 +0.90
#define XTDC4_DC_OFFSET_P_TTL +1.18
#define XTDC4_DC_OFFSET_P_LVTTL_33 +1.18
#define XTDC4_DC_OFFSET_P_LVTTL_25 +1.18
#define XTDC4_DC_OFFSET_P_SSTL_3 +1.18
#define XTDC4_DC_OFFSET_P_SSTL_2 +1.18
#define XTDC4_DC_OFFSET_N_NIM -0.35
#define XTDC4_DC_OFFSET_N_CMOS -1.32
#define XTDC4_DC_OFFSET_N_LVCMOS_33 -1.32
#define XTDC4_DC_OFFSET_N_LVCMOS_25 -1.25
#define XTDC4_DC_OFFSET_N_LVCMOS_18 -0.90
#define XTDC4_DC_OFFSET_N_TTL -1.32
#define XTDC4_DC_OFFSET_N_LVTTL_33 -1.32
#define XTDC4_DC_OFFSET_N_LVTTL_25 -1.25
#define XTDC4_DC_OFFSET_N_SSTL_3 -1.32
#define XTDC4_DC_OFFSET_N_SSTL_2 -1.25
/*!@}*/

/*! \ingroup deftriggersource
 *  @{
 */
#define XTDC4_TRIGGER_SOURCE_S 0x00000001
#define XTDC4_TRIGGER_SOURCE_A 0x00000002
#define XTDC4_TRIGGER_SOURCE_B 0x00000004
#define XTDC4_TRIGGER_SOURCE_C 0x00000008
#define XTDC4_TRIGGER_SOURCE_D 0x00000010
#define XTDC4_TRIGGER_SOURCE_S1 0x00000020
#define XTDC4_TRIGGER_SOURCE_S2 0x00000040
#define XTDC4_TRIGGER_SOURCE_GATE 0x00000080
#define XTDC4_TRIGGER_SOURCE_AUTO 0x00004000
#define XTDC4_TRIGGER_SOURCE_ONE 0x00008000
/*!@}*/

/*! \ingroup packflags
 *  @{
 */
#define XTDC4_PACKET_FLAG_ODD_HITS 1 //!< last 64 bit word contains only one hit
#define XTDC4_PACKET_FLAG_SLOW_SYNC                                            \
    2 //!< Start pulse distance larger than timestamp counter range
#define XTDC4_PACKET_FLAG_START_MISSED                                         \
    4 //!< The trigger unit has discarded packets due to full FIFO
#define XTDC4_PACKET_FLAG_SHORTENED                                            \
    8 //!< The trigger unit has shortend the current packet due to full FIFO
/*! \brief DMA FIFO was full
 *
 *  might not result in dropped packets
 */
#define XTDC4_PACKET_FLAG_DMA_FIFO_FULL 16
/*! \brief Host buffer was full
 *
 *  might not result in dropped packets
 */
#define XTDC4_PACKET_FLAG_HOST_BUFFER_FULL 32
/*!@}*/

/*! \ingroup packflags
 *  @{
 */
#define XTDC4_HIT_FLAG_RISING                                                  \
    1 //!< Timestamp of the rising edge, if not set falling edge
#define XTDC4_HIT_FLAG_TIME_ROLLOVER                                           \
    2 //!< Time since start pulse longer than timestamp counter range
#define XTDC4_HIT_FLAG_COARSE_TIMESTAMP 4 //!< FPGA coarse time
#define XTDC4_HIT_FLAG_TDC_MISSING                                             \
    4 //!< TDC has not provided a timestamp, FPGA coarse time given instead
#define XTDC4_HIT_FLAG_FPGA_MISSING                                            \
    8 //!< FPGA has not seen the stop event, hit may be out of sequence and
      //!< belong to an other group
/*!@}*/

/*! \ingroup funcerrors
 *  @{
 */
/*! \brief no error encountered
 *
 *  used by
 *  - @link defclose xtdc4_close() @endlink
 *  - @link definit xtdc4_init() @endlink
 *  - @link defdefinpar xtdc4_get_default_init_parameters() @endlink
 *  - @link defparaminfo xtdc4_get_param_info() @endlink
 *  - @link defstatinfo xtdc4_get_static_info() @endlink
 *  - @link deffastinfo xtdc4_get_fast_info() @endlink
 *  - @link defdefconf xtdc4_get_default_configuration() @endlink
 *  - @link defcurconf xtdc4_get_current_configuration() @endlink
 */
#define XTDC4_OK 0
#define XTDC4_WINDRIVER_NOT_FOUND 1 //!< NOT IMPLEMENTED
#define XTDC4_DEVICE_NOT_FOUND 2    //!< NOT IMPLEMENTED
#define XTDC4_NOT_INITIALIZED 3     //!< NOT IMPLEMENTED

/*! \brief invalid pointer
 *
 *  The pointer given to a xtdc4 driver function was not a valid pointer
 *  used by
 *  - @link defclose xtdc4_close() @endlink
 *  - @link defstatinfo xtdc4_get_static_info() @endlink
 *  - @link defparaminfo xtdc4_get_param_info() @endlink
 *  - @link deffastinfo xtdc4_get_fast_info() @endlink
 *  - @link defconf xtdc4_configure() @endlink
 *  - @link defdefconf xtdc4_get_default_configuration() @endlink
 *  - @link defcurconf xtdc4_get_current_configuration() @endlink
 *  - @link defstartcap xtdc4_start_capture() @endlink
 *  - @link defstopcap xtdc4_stop_capture() @endlink
 *  - @link defpausecap xtdc4_pause_capture() @endlink
 *  - @link defcontcap xtdc4_continue_capture() @endlink
 *  - @link defack xtdc4_acknowledge() @endlink
 *  - @link defread xtdc4_read() @endlink
 */
#define XTDC4_INVALID_DEVICE 5
#define XTDC4_BUFFER_ALLOC_FAILED 6        //!< NOT IMPLEMENTED
#define XTDC4_TDC_NO_EDGE_FOUND 7          //!< NOT IMPLEMENTED
#define XTDC4_INVALID_BUFFER_PARAMETERS 8  //!< NOT IMPLEMENTED
#define XTDC4_INVALID_CONFIG_PARAMETERS 9  //!< NOT IMPLEMENTED
#define XTDC4_WINDOW_CALIBRATION_FAILED 10 //!< NOT IMPLEMENTED
#define XTDC4_HARDWARE_FAILURE 11          //!< NOT IMPLEMENTED
#define XTDC4_INVALID_TDC_MODE 12          //!< NOT IMPLEMENTED
#define XTDC4_SYNCHRONIZATION_FAILED 13    //!< NOT IMPLEMENTED
#define XTDC4_DEVICE_OPEN_FAILED 14        //!< NOT IMPLEMENTED
/*!@}*/

/*! \ingroup defparaminfo
 */
/*! \brief when a capture on a closed card is called
 *
 * else @link funcerrors XTDC4_OK @endlink
 */
#define XTDC4_WRONG_STATE 4

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
#ifdef XTDC4_DRIVER_EXPORTS
#define XTDC4_API __declspec(dllexport)
#else
#define XTDC4_API __declspec(dllimport)
#endif
#else
#define XTDC4_API
#endif

#define crono_bool_t uint8_t

/*!\ingroup device
 * \brief struct storing properties of the device in use
 */
typedef struct {
    /*! \brief xtdc4
     */
    void *xtdc4;
} xtdc4_device;

/*! \ingroup initparamsstruct
 *  \brief struct for the initialization of the xTDC4
 *
 *  this structure MUST be completely INITIALIZED
 */
typedef tdc4_init_parameters xtdc4_init_parameters;

/*! \ingroup initfuncts
 *  \brief Finalizes the driver for this device.
 *
 *  Return values are listed @link defclose here @endlink.
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 */
XTDC4_API int xtdc4_close(xtdc4_device *device);

/*! \ingroup runtime
 *  \brief Start timing generator
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 */
XTDC4_API int xtdc4_start_tiger(xtdc4_device *device);
/*! \ingroup runtime
 *  \brief Stop timing generator
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 */
XTDC4_API int xtdc4_stop_tiger(xtdc4_device *device);
/*! \ingroup runtime
 *  \brief Start data acquisition
 *
 *  Return values are listed @link defstartcap here @endlink.
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 */
XTDC4_API int xtdc4_start_capture(xtdc4_device *device);
/*! \ingroup runtime
 *  \brief  Pause data acquisition
 *
 *  Return values are listed @link defpausecap here @endlink.
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 */
XTDC4_API int xtdc4_pause_capture(xtdc4_device *device);
/*! \ingroup runtime
 *  \brief resume data acquisition
 *
 *  Call this to resume data acquisition after a call to
 *  xtdc4_pause_capture(). Return values are listed @link defcontcap here
 *@endlink. \param *device is type @link xtdc4_device xtdc4_device @endlink
 */
XTDC4_API int xtdc4_continue_capture(xtdc4_device *device);
/*! \ingroup runtime
 *  \brief  stop data acquisition
 *
 *  Return values are listed @link defstopcap here @endlink.
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 */
XTDC4_API int xtdc4_stop_capture(xtdc4_device *device);

/*! \ingroup runtime
 *  \brief get current state of channel histogram, channel 0 is sync, 1 - 4
 *  channels A - D
 *
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 *  \param *channel is type int
 */
XTDC4_API int xtdc4_get_histogram_state(xtdc4_device *device, int channel);

/*! \ingroup runtime
 *  \brief read given histogram entry, channel 0 is sync, 1 - 4 channels A - D
 *
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 *  \param *channel is type int
 *  \param *entry is type int
 */
XTDC4_API int xtdc4_get_histogram_data(xtdc4_device *device, int channel,
                                       int entry);

/*! \ingroup runtime
 *  \brief start recording histogram, channel 0 is sync, 1 - 4 channels A - D
 *
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 *  \param *channel is type int
 */
XTDC4_API int xtdc4_start_histogram(xtdc4_device *device, int channel);

/*! \ingroup runtime
 *  \brief stop recording histogram, channel 0 is sync, 1 - 4 channels A - D
 *
 *  \param *device is type @link xtdc4_device xtdc4_device @endlink
 *  \param *channel is type int
 */
XTDC4_API int xtdc4_stop_histogram(xtdc4_device *device, int channel);

/*! \ingroup deferror
 *  \brief error code
 *
 *  assignments of the error codes
 *  @{
 */
#define CRONO_READ_OK 0
#define CRONO_READ_NO_DATA 1
#define CRONO_READ_INTERNAL_ERROR 2 //!< Error in buffer handling, must restart
#define CRONO_READ_TIMEOUT 3        //!< Not used yet
/*!@}*/

/*! \ingroup readout
 *  \brief Acknowledges the processing of the last read block.
 *
 *  This is only necessary if xtdc4_read() is not called with
 *  in.acknowledge_last_read set. This feature allows to either free up partial
 *  DMA space early if there will be no call to timetagtimetagger4_read anytime
 *  soon. It also allows to keep data over multiple calls to timetagger4_read to
 *  avoid unnecessary copying of data. Return values are listed @link defack
 *  here
 *  @link defack here @endlink. \param *device is type @link xtdc4_device
 *  xtdc4_device @endlink \param *packet is type crono_packet
 */
XTDC4_API int xtdc4_acknowledge(xtdc4_device *device,
                                volatile crono_packet *packet);

/*! \ingroup staticinfo
 *  \brief Structure contains static information
 *
 *  This structure contains information about the board that does not change
 *  during run time. It is provided by the function @link statfuncts
 *  xtdc4_get_static_info() @endlink.
 */
typedef tdc4_static_info xtdc4_static_info;

/*! \ingroup statfuncts
 *  \brief Contains static information.
 *
 *  Gets a structure that contains information about the board that does not
 *  change during run time. Return values are listed @link defstatinfo here
 *  @endlink. \param *device of type xtdc4_device \param *info of type
 *  xtdc4_static_info
 */
XTDC4_API int xtdc4_get_static_info(xtdc4_device *device,
                                    xtdc4_static_info *info);

/*! \ingroup fastinfo
 *  \brief contains fast dynamic information
 *
 *  This call returns a structure that contains dynamic information that can
 *  be obtained within a few microseconds.
 */
typedef tdc4_fast_info xtdc4_fast_info;

/*! \ingroup statfuncts
 *  \brief Returns fast dynamic information
 *
 *  This call gets a structure that contains dynamic information that can be
 *  obtained within a few microseconds. Return values are listed @link
 *  deffastinfo here @endlink. \param *device of type xtdc4_device \param *info
 *  of type xtdc4_fast_info
 */
XTDC4_API int xtdc4_get_fast_info(xtdc4_device *device, xtdc4_fast_info *info);

/*! \ingroup paraminfo
 *  \brief contains configuration changes
 *
 *  Structure filled by @link statfuncts xtdc4_get_param_info() @endlink.
 *  This structure contains information that change indirectly due to
 *  configuration changes.
 */
typedef tdc4_param_info xtdc4_param_info;

/*! \ingroup statfuncts
 *  \brief returns configuration changes
 *
 *  Gets a structure that contains information that changes indirectly due
 *  to configuration changes. Return values are listed @link defparaminfo here
 *  @endlink. \param *device of type xtdc4_device \param *info of type
 *  xtdc4_device
 */
XTDC4_API int xtdc4_get_param_info(xtdc4_device *device,
                                   xtdc4_param_info *info);

/*! \ingroup readout
 *  \brief returns most recent error message
 *  \param *device is type xtdc4_device
 */
XTDC4_API const char *xtdc4_get_last_error_message(xtdc4_device *device);

/*! \ingroup readout
 *  \brief returns the type of the device
 *
 *  @link devicetype CRONO_DEVICE_XTDC4 @endlink
 */
XTDC4_API int xtdc4_get_device_type(xtdc4_device *device);

/*! \ingroup channel
 *  \brief Contains TDC channel settings
 */
typedef tdc4_channel xtdc4_channel;

/*! \ingroup lowres_channel
 *  \brief Contains digital channel settings
 */
typedef tdc4_lowres_channel xtdc4_lowres_channel;

/*! \ingroup tiger
 *  \brief contains settings of timing generator
 */
typedef tdc4_tiger_block xtdc4_tiger_block;

/* low res trigger config */
/*! \ingroup trigger
 *  \brief contains trigger settings
 */
typedef tdc4_trigger xtdc4_trigger;

/*! \ingroup confstruct Structure xtdc4_configuration
 *  \brief  contains configuration information
 *
 *  This structure contains the configuration information. It is used in
 *  conjunction with
 *  @link conffuncts xtdc4_get_default_configuration(),
 *  xtdc4_get_current_configuration() and xtdc4_configure() @endlink
 *
 *  Internally it uses the structures @link trigger xtdc4_trigger @endlink
 *  and @link tiger xtdc4_tiger_block @endlink
 */
typedef tdc4_configuration xtdc4_configuration;

/*! \ingroup conffuncts
 *  \brief gets default configuration
 *
 *  Copies the default configuration to the specified config pointer. Return
 *  values are listed @link defdefconf here @endlink. \param *device of type
 *  xtdc4_device \param *config of type xtdc4_configuration
 */
XTDC4_API int xtdc4_get_default_configuration(xtdc4_device *device,
                                              xtdc4_configuration *config);
/*! \ingroup conffuncts
 *  \brief gets current configuration
 *
 *  Copies the current configuration to the specified config pointer. Return
 *  values are listed @link defcurconf here @endlink. \param *device of type
 *  xtdc4_device \param *config of type xtdc4_configuration
 */
XTDC4_API int xtdc4_get_current_configuration(xtdc4_device *device,
                                              xtdc4_configuration *config);
/*! \ingroup conffuncts
 *  \brief configures xtdc4 device
 *
 *  Return values are listed @link defconf here @endlink.
 *  \param *device of type xtdc4_device
 *  \param *config of type xtdc4_configuration
 */
/* the config information is copied, so can be changed after the
called */
XTDC4_API int xtdc4_configure(xtdc4_device *device,
                              xtdc4_configuration *config);

/*! \ingroup initfuncts
 *  \brief Returns the number of boards present in the system that are
 *  supported by this driver. \param *error_code is type int \param
 *  **error_message is type
 *  const char
 */
XTDC4_API int xtdc4_count_devices(int *error_code, const char **error_message);

/*! \ingroup initfuncts
 * @{
 */
/*! \brief Sets up the standard parameters
 *
 *  Gets a set of default parameters for @link xtdc4_init xtdc4_init()
 *  @endlink . This must always be used to initialize the @link initparamsstruct
 *  xtdc4_init_parameter() @endlink structure. Return values are listed @link
 *  defdefinpar here @endlink. \param init is type *xtdc4_init_parameters
 */
XTDC4_API int xtdc4_get_default_init_parameters(xtdc4_init_parameters *init);
/*! \brief Open and initialize the XTDC4 board with the given index.
 *
 *  With error_code and error_message the user must provide pointers to
 *  buffers where error information should be written by the driver. Return
 *  values are listed @link definit here @endlink. \param *params type
 *  xtdc4_init_parameters \param *error_code is type int \param
 *  **error_message is type char. The buffer for the error message has to
 *  contain at least 80 chars.
 */
XTDC4_API xtdc4_device *xtdc4_init(xtdc4_init_parameters *params,
                                   int *error_code, const char **error_message);
/*!@}*/

/*! \ingroup readin
 *  \brief The parameters of the read commands
 */
typedef tdc4_read_in xtdc4_read_in;

/*! \ingroup strucreadout
 *  \brief struct for the read out of the packets
 */
typedef tdc4_read_out xtdc4_read_out;

/*! \ingroup readout
 *  \brief Reads packets from the board
 *
 *  optionally automatically acknowledges the last read packets. Return
 *  values are listed @link defread here @endlink. \param *device is type
 *  xtdc4_device \param *in is type xtdc4_read_in \param *out is type
 *  xtdc4_read_out
 */
XTDC4_API int xtdc4_read(xtdc4_device *device, xtdc4_read_in *in,
                         xtdc4_read_out *out);

/*! \ingroup conffuncts
 *  \brief Sets the offsets of FPGA-TDC and main TDC. iserdesOffset[0..3]
 *  are the values for ch A..D.
 *
 *  The values are stored inside the flash PROM, if the calibration PROM is
 *  flashed after setting values. \param *device is type xtdc4_device \param
 *  *iserdesOffset is type short[4]
 */
XTDC4_API int xtdc4_set_CC_offsets(xtdc4_device *device, short *iserdesOffset);

/*! \ingroup conffuncts
 *  \brief Reads the current offsets of FPGA-TDC and main TDC.
 *  iserdesOffset[0..3] are the values for ch A..D.
 *
 *  \param *device is type xtdc4_device
 *  \param *iserdesOffset is type short[4]
 */
XTDC4_API int xtdc4_get_current_CC_offsets(xtdc4_device *device,
                                           short *iserdesOffset);

/*! \ingroup conffuncts
 *  \brief Gets the offsets of FPGA-TDC and main TDC measured using the last
 *  1024 hits recorded. iserdesOffset[0..3] are the values for ch A..D.
 *
 *  \param *device is type xtdc4_device
 *  \param *offsets_new is type bool[4]
 *  \param *iserdesOffset is type short[4]
 */
XTDC4_API int xtdc4_get_measured_CC_offsets(xtdc4_device *device,
                                            bool *offsets_new,
                                            short *iserdesOffset);

/*! \ingroup statfuncts
 *  \brief Returns the driver version, same format as
 *  xtdc4_static_info::driver_revision. This function does not require
 *  a xTDC4 board to be present.
 */
XTDC4_API int xtdc4_get_driver_revision();

/*! \ingroup statfuncts
 *  \brief Returns the driver version including SVN build revision as a
 *  string. This function does not require a xTDC4 board to be present.
 */
XTDC4_API const char *xtdc4_get_driver_revision_str();
#ifdef __cplusplus
}
#endif

#endif
