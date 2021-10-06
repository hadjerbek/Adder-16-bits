//
//  addictionneur.c
//  Additionneur
//
//  Created by Hadjer Bekhira on 18/12/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define N 16 // Number of bit


/**
 * Uses strtol function to convert hexadecimal string to decimal
 * @param strhexa the hexadecimal represtentation in string of
 *        the number to convert 
 * @return the decimal representation of the number
 */
long int convertir_decimal(const char* strhexa) {
    char *endptr;
    // Converts hex to decimal with strtol
    return strtol(strhexa, &endptr, 16);
}


/**
 * Converts the decimal number to binary using bit shift
 * @param tab array of char to store the result
 * @param decimal the decimal number to convert in binary
 */
void convertir_binaire(char* tab, long int decimal)
{
    int i;
    for(i = N - 1; i >= 0; i--) {
    	// We take the i-th bit then do an AND logic with 1 (neutral)
        // index is N-1-i cause the bit at index 0 corresponds to the 
        // bit furthest to the left (most significant bit)
        // += cause tab[i] is the char '0'. += will convert the bit to char
        tab[N-1-i] += ((decimal >> i) & 1);
    }
}


/**
* Check that the string represents a hexadecimal number and that 
* its representation can fit on 16 bits
* @param strhexa the hexadecimal represtentation in string of the
*        number to check
* @return 1 if the string is an hexadecimal number representation
*         0 else
*/
int est_hexa(const char *strhexa)
{
    int i = 0, len = strlen(strhexa), v = 0;
    // Check if the string represents an hexadecimal number
    // First, we verify if the string begin with 0x
    if(len > 2 && strhexa[0] == '0' && strhexa[1] == 'x') i = 2;
    // Then, we verify that each char is beetwen 0-9 and a-f
    while(i < len && (v = isxdigit(strhexa[i++])));
    return v;
}


/**
* Converts an hexadecimal string representation of the number to decimal
* @param tab array of char to store the result
* @param strhexa the hexadecimal represtentation in string of
*        the number to convert
* @return 0 if the conversion works
*         1 if the string is not an hexadecimal representation
*         2 if the hexadecimal number is not representable on 16 bits
*/
int convertir_entree(char* tab, const char* strhexa)
{
    if (est_hexa(strhexa))
    {
        // Hex to decimal conversion
        long int dec = convertir_decimal(strhexa);
		
		// Check if the number can fits on 16 bits. For that, we verify
		// that the number is less than 2^16 = 65535, the maximum number
		// that we can represent in 16 digits binary
		if(dec > 65535) return 2;
        
        // Decimal to binary conversion
        convertir_binaire(tab, dec);
        return 0;
    }
    return 1;
}


/**
 * This function apply the NAND operator, it produces an output which
 * is false only if all its inputs are true
 * @param A  one of the bits to apply the operation
 * @param B  the snd bit to apply the operation
 * @return '1' if A and B equals '1', '0' else
 */
char pl_NAND (char A , char B)
{
    if (A == B && A == '1')
    {
        return '0';
    }
    return '1';
}


/**
 * This function apply the NOR operator, it produces an output which
 * is false only if all its inputs are false
 * @param A  one of the bits to apply the operation
 * @param B  the snd bit to apply the operation
 * @return '1' if A and B equals '0', '1' else
 */
char pl_NOR (char A , char B)
{
    if (A==B && A =='0')
    {
        return '1';
    }
    return '0';
}


/**
 * The combinition of gates NAND and NOR to simulate XOR gate
 * @param A one of the bits to apply the operation
 * @param B the snd bit to apply the operation
 * @return the result of A xor B
 */
char pl_XOR (char A, char B)
{
    char S1 = pl_NAND(A, B);
    char S2 = pl_NAND(S1, A);
    char S3 = pl_NAND(S1, B);
    return pl_NAND(S2, S3);
}


/**
 *The combinition of gates NAND and NOR to simulate AND gate
 * @param A one of the bits to apply the operation
 * @param B the snd bit to apply the operation
 * @return the result of A and B
 */
char pl_AND ( char A, char B)
{
    char S = pl_NAND(A, B);
    return pl_NAND(S,S);
}


/**
 *The combinition of gates NAND and NOR to simulate OR gate
 * @param A one of the bits to apply the operation
 * @param B the snd bit to apply the operation
 * @return the result of A or B
 */
char pl_OR ( char A, char B)
{
    return pl_NAND(pl_NAND(A, A), pl_NAND(B, B));
}


/**
 *The combinition of gates NAND and NOR to simulate NOT gate
 * @param A one of the bits to apply the operation
 * @param B the snd bit to apply the operation
 * @return the result of not A
 */
char pl_NOT ( char A)
{
    return pl_NAND(A, A);
}


/**
 The function compute the addition of two bits A and B and
 * using the Cout pointer to return the output carry.
 * @param A represents one of the two bits to be summed
 * @param B represents the snd bit to be summed
 * @param Cin represents the input carry
 * @param Cout represents the output carry
 * @return The result of addition of the two bits
 */
char add_1b(char A, char B, char Cin , char* Cout)
{
    // Addition of A and B
    char add = pl_XOR(A, B);
    // Get the carry of A+B
    char Ctemp1 = pl_AND(A, B);
    // Add the Input carry to the add
    char res = pl_XOR(add, Cin);
    // Get the carry of the addition of add and the input carry
    char Ctemp2 = pl_AND(add, Cin);
    // Compute the result carry
    *Cout = pl_OR(Ctemp1, Ctemp2);
    // return the result
    return res;
}


/**
 * This function simulates a 16-bit full adder. It fills the array
 * sum of size 16 which represents the sum of the two parameters A and B.
 * @param A represents the first 16 bit to sum
 * @param B represents the snd 16 bits to sum
 * @return 1 if the sum generates an overfllow (i.e. if a 17th bit is
 * needed to represent the result), otherwise 0.
 */
char add_16b(char* A, char* B, char* sum)
{
    // input carry
    char Cin = 0;
    // output carry
    char Cout = '0';
    int i;
    for(i = N-1; i > 0 ; i--)
    {
        // Sum of two bits
        sum[i] = add_1b(A[i], B[i], Cin, &Cout);
        // The output carry of the round become the input carry of the next round
        Cin = Cout;
    }
    if (Cout != '0')
    { // Carry out is 1, so Overflow
        return 1;
    }
    return 0;
}


/**
 * This function converts the array of bits resulting from the addition to an integer
 * @param bits bits array to convert to integer
 * @return integer representation of the bits
 */
u_int16_t convertir_sortie(char* bits)
{
    char *endptr;
    // Converts bin to decimal with strtol
    return strtol(bits, &endptr, 2);
}


int main(int argc, const char * argv[]) {
    if (argc != 3)
    {
        perror("You need to specify exactly two arguments : the two numbers to sum.\n\n");
        exit(2);
    }
    char* A = (char*)malloc(sizeof(char) * (N+1));
    char* B = (char*)malloc(sizeof(char) * (N+1));
    char* sum = (char*)malloc(sizeof(char) * (N+1));
    int i, r, success = 1;
    // Put '0' in each case of the arrays
    for(i=0; i<N; i++)
    {
        A[i] = '0';
        B[i] = '0';
        sum[i] = '0';
    }
    // To convert A and B into a string
    A[N]='\0';
    B[N]='\0';
    sum[N]='\0';
    
    // Converts arg 1 to bin
    r = convertir_entree(A, argv[1]);
    if(r == 1) 
    {
        printf("Argument %s is not an hexadecimal number representation\n", argv[1]);
        success = 0;
    } else if (r == 2) 
    {
	    printf("The parameter %s cannot be represented on 16 bits\n", argv[1]);
	    success = 0;
    }
    // Converts arg 2 to bin
    r = convertir_entree(B, argv[2]);
    if(r == 1) 
    {
        printf("Argument %s is not an hexadecimal number representation\n\n", argv[2]);
        success = 0;
    } else if (r == 2)
    {
	    printf("The parameter %s cannot be represented on 16 bits\n\n", argv[2]);
	    success = 0;
    }

    // Conversions works
    if(success) {
        r = add_16b(A, B, sum);
        printf("%s + %s = %x (%s) overflow %d\n\n", A, B, convertir_sortie(sum), sum, r);
    }

    // Free the memories
    free(A);
    free(B);
    free(sum);

    return 0;
}
