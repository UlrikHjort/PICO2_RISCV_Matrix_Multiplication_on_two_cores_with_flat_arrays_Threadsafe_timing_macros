/**************************************************************************
--        PICO2 RISC-V multicore matrix multiplication flat arrays  
-- 
--           Copyright (C) 2025 By Ulrik Hørlyk Hjort
--
--  This Program is Free Software; You Can Redistribute It and/or
--  Modify It Under The Terms of The GNU General Public License
--  As Published By The Free Software Foundation; Either Version 2
--  of The License, or (at Your Option) Any Later Version.
--
--  This Program is Distributed in The Hope That It Will Be Useful,
--  But WITHOUT ANY WARRANTY; Without Even The Implied Warranty of
--  MERCHANTABILITY or FITNESS for A PARTICULAR PURPOSE.  See The
--  GNU General Public License for More Details.
--
-- You Should Have Received A Copy of The GNU General Public License
-- Along with This Program; if not, See <Http://Www.Gnu.Org/Licenses/>.
***************************************************************************/
#include "pico/stdlib.h"
#include <stdint.h>
#include <stdio.h>
#include "pico/multicore.h"
#include "hardware/structs/sio.h"
#include "uart_pio.h"
#include "timing_multicore.h"

#define UART_TX_PIN 2


#define M 6   // rows of A
#define N 4   // cols of A / rows of B
#define P 5   // cols of B



int A[M * N];
int B[N * P];
int C[M * P];

void print_matrix(const char *name, int *m, int rows, int cols) {
    printf("%s (%d x %d):\n\r", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", m[i * cols + j]);  // row-major indexing
        }
        printf("\n\r");
    }
    printf("\n\r");
}

// Core 1 computes rows M/2 .. M-1
void core1_task() {
	TIME_START()
    for (int i = M/2; i < M; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i*N + k] * B[k*P + j];
            }
            C[i*P + j] = sum;
        }
    }
	TIME_US_STOP("Multi core run");
    multicore_fifo_push_blocking(1);  // signal done
}

int multicore_run() {

    // Fill A and B with example values
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            A[i*N + j] = (i + j + 1);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < P; j++)
            B[i*P + j] = (i * j + 1);

    print_matrix("A", A, M, N);
    print_matrix("B", B, N, P);


    // Start core 1
    multicore_launch_core1(core1_task);
	TIME_START();
    // CORE 0: compute first half of rows
    for (int i = 0; i < M/2; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i*N + k] * B[k*P + j];
            }
            C[i*P + j] = sum;
        }
    }
	TIME_US_STOP("Multi core run");

    // Wait for core 1
    multicore_fifo_pop_blocking();

    print_matrix("C = A × B", C, M, P);

    return 0;
}



int singlecore_run() {

    // Fill A and B with example values
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            A[i*N + j] = (i + j + 1);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < P; j++)
            B[i*P + j] = (i * j + 1);

    print_matrix("A", A, M, N);
    print_matrix("B", B, N, P);


	TIME_START();
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i*N + k] * B[k*P + j];
            }
            C[i*P + j] = sum;
        }
    }
	TIME_US_STOP("Single core run");


    print_matrix("C = A × B", C, M, P);

    return 0;
}



int main() {
		init_uart(19200,UART_TX_PIN);
		multicore_run();
		singlecore_run();
}
