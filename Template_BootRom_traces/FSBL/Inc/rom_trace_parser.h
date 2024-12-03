/* ===========================================================================*/
/*! @file rom_trace_parser.h
    @author esagnard
*/
/* ===========================================================================
 * (C) Copyright 2019 ST Microelectronics, All rights reserved
 *
 * This source code and any compilation or derivative thereof is the sole
 * property of ST Microelectronics Corporation and is provided pursuant to a Software
 * License Agreement. This code is the proprietary information of ST Microelectronics
 * Corporation and is confidential in nature. Its use and dissemination by
 * any party other than ST Microelectronics Corporation is strictly limited by the
 * confidential information provisions of the Agreement referenced above.
 * ===========================================================================
 *
 *   DESCRIPTION:
 *   ------------
 *  This file is the API of Wildcat ROM trace parser
 *============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*============================================================================*/
/* Exported constants                                                         */
/*============================================================================*/

#define ROM_TRACE_ERR_OK               (0)
#define ROM_TRACE_ERR_KO               (-1)
#define ROM_TRACE_ERR_BAD_ID           (-2)
#define ROM_TRACE_ERR_NO_START_WORD    (-3)
#define ROM_TRACE_ERR_WRONG_START_WORD (-4)
#define ROM_TRACE_ERR_WRONG_SIZE       (-5)
#define ROM_TRACE_ERR_WRONG_LEVEL      (-6)
#define ROM_TRACE_ERR_END_OF_BUFFER    (-7)

#define ROM_TRACE_ERR_STR(err) \
        (((err)==ROM_TRACE_ERR_OK)? "OK" : \
        (((err)==ROM_TRACE_ERR_KO)? "KO" : \
        (((err)==ROM_TRACE_ERR_BAD_ID)? "BAD_ID" : \
        (((err)==ROM_TRACE_ERR_NO_START_WORD)? "NO_START_WORD" : \
        (((err)==ROM_TRACE_ERR_WRONG_START_WORD)? "WRONG_START_WORD" : \
        (((err)==ROM_TRACE_ERR_WRONG_SIZE)? "WRONG_SIZE" : \
        (((err)==ROM_TRACE_ERR_WRONG_LEVEL)? "WRONG_LEVEL" : \
        (((err)==ROM_TRACE_ERR_END_OF_BUFFER)? "END_OF_BUFFER" : \
         "UNKNOWN" ))))))))


/*============================================================================*/
/* Exported types                                                             */
/*============================================================================*/
typedef struct {
    uint8_t *p_trace_buffer;
    uint32_t trace_buffer_size;
    uint32_t *p_cur_trace;
    uint32_t remaining_size;

} romTrace_Iterator_t;





/*============================================================================*/
/* Exported functions                                                         */
/*============================================================================*/



/*============================================================================*/
/**
    @brief
    This function starts ROM trace iteration.

    It initializes itetator structure and looks for start word.

    @param[in]             iterPtr: pointer to trace iterator
    @param[in]             inPtr: Pointer to a binary trace buffer
    @param[in]             inSize: size of binary trace buffer

    @return
    - ROM_TRACE_ERR_OK if everything went well
    - ROM_TRACE_ERR_WRONG_SIZE    if given size is not a multiple of 4 bytes
    - ROM_TRACE_ERR_NO_START_WORD if function cannot find start word.

*/
/*============================================================================*/
extern int32_t romTrace_IterateInit(romTrace_Iterator_t *iterPtr, uint8_t *inPtr, uint32_t inSize);


/*============================================================================*/
/**
    @brief This function convert next trace into text.

    @param[in]             iterPtr: pointer to trace iterator
    @param[out]            outPtr: Output string containing the parsed trace
    @param[out]            timestamp: timestamp use to sort traces
    @param[in]             outSize: maximum outPtr size

    @return
    - ROM_TRACE_ERR_OK            if everything went well
    - ROM_TRACE_ERR_WRONG_SIZE    if given size is not a multiple of 4 bytes
    - ROM_TRACE_ERR_NO_START_WORD if function cannot find start word.

*/
/*============================================================================*/
extern int32_t romTrace_IterateNext(romTrace_Iterator_t *iterPtr, char *outPtr, uint32_t *timestamp, uint32_t outSize);

/*============================================================================*/
/**
    @brief
    This function will convert a trace info text.

    @param[in]             inPtr: Pointer to a trace
    @param[in]             inSize: size of input buffer
    @param[out]            outPtr: Output string containing the parsed trace
    @param[in]             sizeMax: maximum outPtr size

    @return
    - ROM_TRACE_ERR_OK if everything went well

*/
/*============================================================================*/


extern int32_t romTrace_Parse (uint8_t *inPtr, int32_t inSize, char * outPtr, uint32_t sizeMax);

/*============================================================================*/
/* End of File                                                                */
/*============================================================================*/

#ifdef __cplusplus
}
#endif
