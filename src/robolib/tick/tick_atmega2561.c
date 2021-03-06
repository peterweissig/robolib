/******************************************************************************
* tick/tick_atmega2561.c                                                      *
* ======================                                                      *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

//**************************<File version>*************************************
#define ROBOLIB_TICK_SUB_VERSION \
  "robolib/tick/tick_atmega2561.c 20.03.2018 V1.0.1"

//**************************[tick_init]**************************************** 27.09.2015
void robolib_tick_init() {

    #if TICK_SYSTICK == TIMER0 // switch TIMER

        #if   F_CPU / 100 / 1024 > 256
            #error "can't setup systick with TIMER0 - prescaler maximum is 1024"

        #elif F_CPU / 100 /  256 > 256
            #define ROBOLIB_TICK_CS  0b101
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 / 1024))

        #elif F_CPU / 100 /   64 > 256
            #define ROBOLIB_TICK_CS  0b100
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /  256))

        #elif F_CPU / 100 /    8 > 256
            #define ROBOLIB_TICK_CS  0b011
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /   64))

        #elif F_CPU / 100 /    1 > 256
            #define ROBOLIB_TICK_CS  0b010
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /    8))

        #else
            #define ROBOLIB_TICK_CS  0b001
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /    1))
        #endif

        // 8-bit Timer
        // Mode 2 (CTC til OCRA)
        TCCR0A =  _BV(WGM01);
            // Bit 4-7 (COM0xx) = 0000 output mode for A and B
            // Bit 0-1 (WGM0x ) =  010 select timer mode [WGM02 in TCCR0B]

        TCCR0B =  (ROBOLIB_TICK_CS & 0x07);
            // Bit 6-7 (FOC0x ) =   00 force output compare
            // Bit 3   (WGM02 ) =    0 [see WGM0x in TCCR0A]
            // Bit 0-2 (CS02  ) =  ??? [calculated]

        TCNT0 = 0;
            // Timer/Counter Register - current value of timer

        OCR0A = ROBOLIB_TICK_MAX;
            // Output Compare Register A - top for timer

        OCR0B = 0;
            // Output Compare Register B - unused

        TIMSK0 = _BV(OCIE0A);
            // Bit 2   (OCIE0B) =    0 interrupt for compare match B
            // Bit 1   (OCIE0A) =    1 interrupt for compare match A (tick)
            // Bit 0   (TOIE0 ) =    0 interrupt for overflow

        TIFR0 = _BV(OCF0A) | _BV(OCF0B) | _BV(TOV0);
            // Bit 1-2 (OCF0x ) =   11 interrupt flag for compare match x
            // Bit 0   (TOV0  ) =    1 interrupt flag for overflow

        #define ROBOLIB_TICK_ISR ISR(TIMER0_COMPA_vect)
        #define ROBOLIB_TICK_ISR_SEI() ( \
           { TIMSK0&= ~ _BV(OCIE0A); sei();})
        #define ROBOLIB_TICK_ISR_CLI() ( \
           { cli(); TIMSK0|= _BV(OCIE0A);})

    #else                                 // switch TICK_SYSTICK
        #error "timer is not supported (yet) for this device"
    #endif                                // switch TICK_SYSTICK
}

//**************************[ISR(tick)]**************************************** 20.03.2018
ROBOLIB_TICK_ISR {

    robolib_tick_time++;

    ROBOLIB_TICK_ISR_SEI();

    #ifdef ROBOLIB_TICK_FUNCTION
        ROBOLIB_TICK_FUNCTION();
    #endif;

    tick_userfunction();

    ROBOLIB_TICK_ISR_CLI();
}
