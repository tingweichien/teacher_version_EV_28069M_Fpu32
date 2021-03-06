/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#ifndef _THROTTLE_H_
#define _THROTTLE_H_

//! \file   ~sw/modules/throttle/src/float/throttle.h
//! \brief  Contains the public interface to the 
//!         math (MATH) module routines
//!
//! (C) Copyright 2011, Texas Instruments, Inc.


// **************************************************************************
// the includes

#include "sw/modules/types/src/types.h"
#include "sw/modules/math/src/32b/math.h"
#include "sw/Sources/include/filter.h"


//!
//!
//! \defgroup THROTTLE THROTTLE
//!
//@{

// Include the algorithm overview defined in modules/<module>/docs/doxygen/doxygen.h
//! \defgroup THROTTLE_OVERVIEW 


#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the defines


// **************************************************************************
// the typedefs

//! \brief Enumeration for the throttle state machine
//!
typedef enum
{
  Throttle_CalMaxMin = 0,
  Throttle_CalCalc = 1,
  Throttle_Run = 2
} Throttle_States_e;




//! \brief      Defines the encoder object
//! \details    The encoder object takes in data from a QEP peripheral and calculates speed
//!             and position.
//!
typedef struct _Throttle_Obj_
{
  _iq                 iqMax_adc;                  //!<Maximum value that is measured from the throttle.
  _iq                 iqMin_adc;                  //!<Minimum value that is measured from the throttle.
  _iq                 iqMax_out;                  //!<Maximum value that is output as a result.
  _iq                 iqMin_out;                  //!<Minimum value that is output as a result.
  _iq                 iqSlope;                    //!<Slope of y=mx+b of the throttle.
  _iq                 iqOffset;                   //!<Offset of y=mx+b of the throttle.
  _iq                 iqValue;                    //!<The input value (x) of the throttle.
  _iq                 iqResult;                   //!<The output corrected result of the throttle module.
  bool                bFlagSw1;                  //!<Switch to put the throttle into max and min calibration mode.
  bool                bFlagSw2;                  //!<Switch to take the throttle out of max and min calibration mode.
  Throttle_States_e   state;                    //!<The state of the throttle.

  FILTER_Obj          filterObj;
  FILTER_Handle       filterHandle;
} Throttle_Obj;


//! \brief      Defines the THROTTLE handle
//! \details    The THROTTLE handle is a pointer to a THROTTLE object.  In all THROTTLE functions
//!             the THROTTLE handle is passed so that the function knows what peripherals
//!             are to be accessed.
//!
typedef struct _Throttle_Obj_ *Throttle_Handle;


// **************************************************************************
// the function prototypes


//! \brief     Initializes the throttle module
//! \param[in] pMemory      A pointer to the memory for the throttle object
//! \param[in] numBytes     The number of bytes allocated for the throttle, bytes
//! \return                 The encoder (THROTTLE) object handle
extern Throttle_Handle Throttle_init(void *pMemory, const size_t numBytes);


//! \brief     Sets up the throttle module parameters initially
//! \param[in] handle  The throttle handle
//! \param[in] handle  The maximum adc measured value in per unit
//! \param[in] handle  The minimum adc measured value in per unit
//! \param[in] handle  The maximum output result in per unit
//! \param[in] handle  The minimum output result in per unit
extern void Throttle_setParams(Throttle_Handle handle,  \
                                const bool binvert,      \
                                const _iq iqMax_adc,      \
                                const _iq iqMin_adc,      \
                                const _iq iqMax_out,      \
                                const _iq iqMin_out);


extern void Throttle_setup(Throttle_Handle handle,      \
                                const _iq iqValue,        \
                                const bool bSW1,         \
                                const bool bSW2);

                                
extern void Throttle_runState(Throttle_Handle handle);


static inline _iq Throttle_getResult(Throttle_Handle handle)
{
	Throttle_Obj *pObj = (Throttle_Obj *)handle;

	return pObj->iqResult;
}

static inline _iq Throttle_getFilterResult(Throttle_Handle handle)
{
    Throttle_Obj *pObj = (Throttle_Obj *)handle;
    return FILTER_getValue(pObj->filterHandle);
}

extern void Throttle_run(Throttle_Handle handle, _iq iqValue);

#endif
