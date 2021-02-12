#include <stdio.h>

#include <stdlib.h>
int error_code = 0;
int output_size = 0;    //length of output string

/*   FUNCTIONS    */
/*  get line character by character */
int get_line(char ** line);
/*  compare distance between each two corresponding characters in both arrays and feed it to "rotate"   */
int compare(const char * coded_part, const char * reference, int coded_part_size);
/*  put the letter and get shifted one    */
int shift_letter(int letter, int offset);
/*  shifts elements of the first string with offset and saves the value to the second string */
void shift(const char * coded_part, char * result_string, int offset, int size);





int get_line(char ** line){
    int received_char;
    int cell_number=0;
    int size=1;
    int length=0;

    while ((received_char = getchar()) != EOF) {
        if (received_char == 10) {
            break;
        }
        if (received_char == 94) {
            break;
        }
        if (received_char == 13) {
            continue;
        }
        if ((received_char >= 'A' && received_char <= 'Z') || (received_char >= 'a' && received_char <= 'z')) {
            if (cell_number == size) {
                char * temp = NULL;
                temp = (char*) realloc(*line, size * 2 * sizeof(char));
                if (temp == NULL) {
                    free(line);
                    return 0;
                }else{
                    *line = temp;
                }
                size *= 2;
            }
            (*line)[cell_number] = received_char;
            length++;
            cell_number++;
        } else {
            fprintf(stderr, "Error: Chybny vstup!\n");
            error_code = 100;
            return 0;
        }
    }
    return length;
}

int compare(const char * coded_part, const char * reference, int coded_part_size){
    int occurence = 0;
    for(int i = 0; i < coded_part_size; i++){
        if(coded_part[i] == reference[i]){
            occurence++;
        }
    }
    return occurence;
}

int shift_letter(int letter, int offset){
    char alphabet[52] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
                         'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                         'a','b','c','d','e','f','g','h','i','j','k','l','m',
                         'n','o','p','q','r','s','t','u','v','w','x','y','z'};
    int i;
    if(offset == 0){
        return letter;
    }
    if(letter <= 'Z')   //  Correcting the ASCII value to be counted from 0
        i = letter - 'A';
    else
        i = letter - 'a' + 26;
    if(i+offset >= 52)
        return alphabet[i+offset - 52];
    else
        return alphabet[i+offset];
}

void shift(const char * coded_part, char * result_string, int offset, int size){
    //shifts elements of the first string with offset and saves the value to the second string
    for(int i = 0; i < size; i++){
        result_string[i] = shift_letter(coded_part[i], offset);
    }
}

int main() {
    int length_coded = 0;
    int length_reference = 0;
    char *coded_part = malloc(10* sizeof *coded_part);  //  Allocating coded_part
    if(coded_part == NULL){  //  Check if allocation was successful
        free(coded_part);
        return 103;
    }

    char *reference = malloc(10* sizeof *reference);    //  Allocating reference
    if(reference == NULL){  //  Check if allocation was successful
        free(reference);
        free(coded_part);
        return 103;
    }
    length_coded = get_line(&coded_part);   //  Functions return length of received string
    length_reference = get_line(&reference);

    if(length_coded == 0 || length_reference == 0){     //  If one of functions returned 0, deallocate
        free(reference);
        free(coded_part);
        return(error_code);
    }

    if(length_coded != length_reference){   //  Check if strings have the same length
        fprintf(stderr, "Error: Chybna delka vstupu!\n");
        free(reference);
        free(coded_part);
        return 101;
    }

    output_size = length_coded; //  Setting the output size

    int offset_occurence[52];
    int max_offset = 0;
    int max=0;
    for(int offset = 0; offset < 52; offset++){
        //  Create a shifted string for every possible offset (52 variants, all small and all large letters),
        //  then check the occurrence of each one by comparing it with "reference" string elements and determine
        //  the most often met offset
        char * shifted_str = (char*)malloc(output_size * sizeof(char));
        shift(coded_part, shifted_str, offset, output_size);
        offset_occurence[offset] = compare(shifted_str, reference, output_size);
        free(shifted_str);
        if (offset_occurence[offset] > max){
            max = offset_occurence[offset];
            max_offset = offset;
        }
    }

    char * output = (char*)malloc(output_size*sizeof(char));
    shift(coded_part, output, max_offset, output_size);
    for (int i = 0; i < output_size; i++){
        printf("%c", output[i]);
    }
    printf("\n");

    free(reference);
    free(coded_part);
    free(output);

    return error_code;
}
