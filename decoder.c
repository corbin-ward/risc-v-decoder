#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Function to extract bits from instruction
uint32_t extract_bits(uint32_t instruction, int start, int length) {
    return (instruction >> start) & ((1 << length) - 1);
}

// Function to sign extend a value
int32_t sign_extend(uint32_t value, int bit_width) {
    // Get the sign bit
    int32_t sign_bit = value >> (bit_width - 1);
    
    if (sign_bit) {
        // Calculate how many bits to shift
        int shift_amount = 32 - bit_width;
        // Shift left to clear the upper bits
        // Then shift right to fill with 1s
        return ((int32_t)value << shift_amount) >> shift_amount;
    }
    return value;
}

// Function to decode R-type instructions
void decode_r_type(uint32_t instruction) {
    uint32_t rd = extract_bits(instruction, 7, 5);
    uint32_t funct3 = extract_bits(instruction, 12, 3);
    uint32_t rs1 = extract_bits(instruction, 15, 5);
    uint32_t rs2 = extract_bits(instruction, 20, 5);
    uint32_t funct7 = extract_bits(instruction, 25, 7);
    
    printf("Instruction Type: R\n");
    
    // Determine operation based on funct3 and funct7
    if (funct3 == 0 && funct7 == 0) printf("Operation: add\n");
    else if (funct3 == 0 && funct7 == 0x20) printf("Operation: sub\n");
    else if (funct3 == 7 && funct7 == 0) printf("Operation: and\n");
    else if (funct3 == 6 && funct7 == 0) printf("Operation: or\n");
    else if (funct3 == 1 && funct7 == 0) printf("Operation: sll\n");
    else if (funct3 == 2 && funct7 == 0) printf("Operation: slt\n");
    else if (funct3 == 3 && funct7 == 0) printf("Operation: sltu\n");
    else if (funct3 == 5 && funct7 == 0) printf("Operation: srl\n");
    else if (funct3 == 5 && funct7 == 0x20) printf("Operation: sra\n");
    else if (funct3 == 4 && funct7 == 0) printf("Operation: xor\n");
    
    printf("Rs1: x%u\n", rs1);
    printf("Rs2: x%u\n", rs2);
    printf("Rd: x%u\n", rd);
    printf("Funct3: %u\n", funct3);
    printf("Funct7: %u\n", funct7);
}

// Function to decode I-type instructions
void decode_i_type(uint32_t instruction) {
    uint32_t rd = extract_bits(instruction, 7, 5);
    uint32_t funct3 = extract_bits(instruction, 12, 3);
    uint32_t rs1 = extract_bits(instruction, 15, 5);
    uint32_t imm = extract_bits(instruction, 20, 12);
    int32_t signed_imm = sign_extend(imm, 12);
    
    printf("Instruction Type: I\n");
    
    // Determine operation based on funct3
    if (funct3 == 7) printf("Operation: andi\n");
    else if (funct3 == 6) printf("Operation: ori\n");
    else if (funct3 == 0) printf("Operation: addi\n");
    else if (funct3 == 1) printf("Operation: slli\n");
    else if (funct3 == 2) printf("Operation: slti\n");
    else if (funct3 == 3) printf("Operation: sltiu\n");
    else if (funct3 == 5) {
        uint32_t funct7 = extract_bits(instruction, 25, 7);
        if (funct7 == 0) printf("Operation: srli\n");
        else printf("Operation: srai\n");
    }
    else if (funct3 == 4) printf("Operation: xori\n");
    
    printf("Rs1: x%u\n", rs1);
    printf("Rd: x%u\n", rd);
    printf("Immediate: %d (or 0x%X)\n", signed_imm, (unsigned int)(signed_imm & 0xFFF));
}

// Function to decode S-type instructions
void decode_s_type(uint32_t instruction) {
    uint32_t imm_low = extract_bits(instruction, 7, 5);
    uint32_t funct3 = extract_bits(instruction, 12, 3);
    uint32_t rs1 = extract_bits(instruction, 15, 5);
    uint32_t rs2 = extract_bits(instruction, 20, 5);
    uint32_t imm_high = extract_bits(instruction, 25, 7);
    uint32_t imm = (imm_high << 5) | imm_low;
    int32_t signed_imm = sign_extend(imm, 12);
    
    printf("Instruction Type: S\n");
    
    // Determine operation based on funct3
    if (funct3 == 0) printf("Operation: sb\n");
    else if (funct3 == 1) printf("Operation: sh\n");
    else if (funct3 == 2) printf("Operation: sw\n");
    
    printf("Rs1: x%u\n", rs1);
    printf("Rs2: x%u\n", rs2);
    printf("Immediate: %d (or 0x%X)\n", signed_imm, (unsigned int)(signed_imm & 0xFFF));
}

// Function to decode SB-type instructions
void decode_sb_type(uint32_t instruction) {
    uint32_t imm_11 = extract_bits(instruction, 7, 1);
    uint32_t imm_4_1 = extract_bits(instruction, 8, 4);
    uint32_t funct3 = extract_bits(instruction, 12, 3);
    uint32_t rs1 = extract_bits(instruction, 15, 5);
    uint32_t rs2 = extract_bits(instruction, 20, 5);
    uint32_t imm_10_5 = extract_bits(instruction, 25, 6);
    uint32_t imm_12 = extract_bits(instruction, 31, 1);
    
    uint32_t imm = (imm_12 << 12) | (imm_11 << 11) | (imm_10_5 << 5) | (imm_4_1 << 1);
    int32_t signed_imm = sign_extend(imm, 13);
    
    printf("Instruction Type: SB\n");
    
    // Determine operation based on funct3
    if (funct3 == 0) printf("Operation: beq\n");
    else if (funct3 == 1) printf("Operation: bne\n");
    else if (funct3 == 5) printf("Operation: bge\n");
    else if (funct3 == 4) printf("Operation: blt\n");
    
    printf("Rs1: x%u\n", rs1);
    printf("Rs2: x%u\n", rs2);
    printf("Immediate: %d\n", signed_imm);
}

// Function to decode UJ-type instructions
void decode_uj_type(uint32_t instruction) {
    uint32_t rd = extract_bits(instruction, 7, 5);
    uint32_t imm_19_12 = extract_bits(instruction, 12, 8);
    uint32_t imm_11 = extract_bits(instruction, 20, 1);
    uint32_t imm_10_1 = extract_bits(instruction, 21, 10);
    uint32_t imm_20 = extract_bits(instruction, 31, 1);
    
    // Construct immediate value correctly for JAL
    uint32_t imm = (imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1);
    int32_t signed_imm = sign_extend(imm, 21);
    
    printf("Instruction Type: UJ\n");
    printf("Operation: jal\n");
    printf("Rd: x%u\n", rd);
    printf("Immediate: %d (or 0x%X)\n", signed_imm, (unsigned int)(signed_imm & 0xFFFFF));
}

int main() {
    char input[33];  // 32 bits + null terminator
    uint32_t instruction;
    
    while (1) {
        printf("Enter an instruction:\n");
        if (scanf("%s", input) != 1) break;
        
        // Convert binary string to integer
        instruction = 0;
        for (int i = 0; i < 32; i++) {
            if (input[i] == '1') {
                instruction |= (1U << (31 - i));
            }
        }
        
        // Get opcode directly from input string
        uint32_t opcode = 0;
        for (int i = 25; i <= 31; i++) {
            opcode = (opcode << 1) | (input[i] - '0');
        }
        
        // Decode based on instruction type
        switch (opcode) {
            case 0x33:  // R-type
                decode_r_type(instruction);
                break;
            case 0x13:  // I-type arithmetic
            case 0x03:  // I-type load
            case 0x67:  // I-type jalr
                decode_i_type(instruction);
                break;
            case 0x23:  // S-type
                decode_s_type(instruction);
                break;
            case 0x63:  // SB-type
                decode_sb_type(instruction);
                break;
            case 0x6F:  // UJ-type
                decode_uj_type(instruction);
                break;
            default:
                printf("Unknown instruction type (opcode=0x%x)\n", opcode);
                break;
        }
        printf("\n");
    }
    
    return 0;
}