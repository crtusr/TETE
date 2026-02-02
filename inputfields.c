#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "inputfields.h"

#define IS_EXTENDED_PRINT(c) ((unsigned char)(c) >= 32 && (unsigned char)(c) <= 255)


static int calculate_date_display_cursor_pos(const InputField* field) {
    // input_buffer stores DDMMYYYY. field->cursor_pos is index in this buffer.
    // Display format is YYYY-MM-DD.
    // map_buffer_idx_to_display_x: maps DDMMYYYY buffer index to display X coordinate
    //   Input_buffer indices: D1=0, D2=1, M1=2, M2=3, Y1=4, Y2=5, Y3=6, Y4=7
    //   Display X positions for these digits: Y1=0, Y2=1, Y3=2, Y4=3, M1=5, M2=6, D1=8, D2=9
    int map_buffer_idx_to_display_x[8] = {
        0, // D1 (buffer_idx 0) -> display_x 8
        1, // D2 (buffer_idx 1) -> display_x 9
        3, // M1 (buffer_idx 2) -> display_x 5
        4, // M2 (buffer_idx 3) -> display_x 6
        6, // Y1 (buffer_idx 4) -> display_x 0
        7, // Y2 (buffer_idx 5) -> display_x 1
        8, // Y3 (buffer_idx 6) -> display_x 2
        9  // Y4 (buffer_idx 7) -> display_x 3
    };

    int display_cursor_x;

    if (field->count == 0) { // No input yet
        display_cursor_x = 0; // Default to start of YYYY (display X position 0)
    } else if (field->cursor_pos == field->count) { // Cursor is at the end of the current input
        // Position cursor logically after the character at input_buffer[field->cursor_pos - 1]
        int buffer_idx_of_last_char = field->cursor_pos - 1;
        // Ensure buffer_idx_of_last_char is valid before using it as an array index
        if (buffer_idx_of_last_char >= 0 && buffer_idx_of_last_char < 8) {
            display_cursor_x = map_buffer_idx_to_display_x[buffer_idx_of_last_char] + 1;
        } else {
             display_cursor_x = 0; // Fallback, e.g. if cursor_pos was 0 but count > 0 (should not happen if cursor_pos=count)
        }
    } else { // Cursor is logically before the character at input_buffer[field->cursor_pos]
        // Ensure field->cursor_pos is valid
        if (field->cursor_pos >= 0 && field->cursor_pos < 8) {
            display_cursor_x = map_buffer_idx_to_display_x[field->cursor_pos];
        } else {
            display_cursor_x = 0; // Fallback
        }
    }
    return display_cursor_x;
}

void init_input_field(InputField* field, const char* prompt, int max_length, bool password_mode, int start_x, int start_y, int type) {
    field->prompt = strdup(prompt);
    field->max_length = max_length;
    field->password_mode = password_mode;
    field->start_x = start_x;
    field->start_y = start_y;
    memset(field->input_buffer, 0, sizeof(field->input_buffer));
    field->count = 0;
    field->cursor_pos = 0;
    field->type = type;
}



void draw_input_field(const InputField* field) 
{
  move(field->start_y, field->start_x);
  //clrtoeol();
  mvprintw(field->start_y, field->start_x, "%s", field->prompt);
  attron(A_REVERSE);
	for (int i = 0; i < field->max_length; i++) 
	{
		addch(' ');
	}
	move(field->start_y, field->start_x + strlen(field->prompt));
	for (int i = 0; i < field->count; i++) 
	{
    if (field->password_mode) 
		{
      addch('*');
    } 
		else
		{
      addch((unsigned char)field->input_buffer[i]);
    }
  }
  attroff(A_REVERSE);
  move(field->start_y, field->start_x + strlen(field->prompt) + field->cursor_pos); // Posicionar el cursor
}

void draw_date_field(const InputField* field)
{
    // input_buffer stores DDMMYYYY. datePos maps display order to buffer.
    // datePos[k] is the index in input_buffer for the k-th digit of YYYYMMDD.
    int datePos[8] = {0, 1, 2, 3, 4, 5, 6, 7}; 

    char display_str[11]; // For "YYYY-MM-DD" (10 chars) + null terminator
    const int display_len = 10;

    // Initialize display string with spaces and separators
    for(int i=0; i<display_len; ++i) display_str[i] = ' '; // Default to space
    display_str[2] = '-';
    display_str[5] = '-';
    display_str[display_len] = '\0';

    // Populate display_str with digits from input_buffer
    for (int k = 0; k < 8; ++k) { // k = 0 for Y1, ..., k = 7 for D2
        int buffer_idx = datePos[k]; // Index in DDMMYYYY input_buffer for this digit

        int current_display_char_pos = k; // Base index in a compact YYYYMMDD sequence
        if (k >= 2) current_display_char_pos++; // Adjust for first '-'
        if (k >= 4) current_display_char_pos++; // Adjust for second '-'

        if (buffer_idx < field->count && field->input_buffer[buffer_idx] != '\0') {
            display_str[current_display_char_pos] = field->input_buffer[buffer_idx];
        }
        // else: it remains ' '
    }

    // Drawing
    move(field->start_y, field->start_x);
    //clrtoeol();
    mvprintw(field->start_y, field->start_x, "%s", field->prompt);

    attron(A_REVERSE);
    // printw is relative to current cursor position, use mvprintw or move first for the content part
    mvprintw(field->start_y, field->start_x + strlen(field->prompt), "%s", display_str);
    attroff(A_REVERSE);
    // Cursor positioning is now handled by input_fields_loop
}

static void insert_char(InputField* field, char ch) 
{
    if (field->count < field->max_length) 
    {
        // Use memmove for efficient shifting of the buffer
        memmove(&field->input_buffer[field->cursor_pos + 1],
                &field->input_buffer[field->cursor_pos],
                field->count - field->cursor_pos);

        field->input_buffer[field->cursor_pos] = ch;
        field->count++;
        field->cursor_pos++;
        field->input_buffer[field->count] = '\0'; // Ensure null termination
    }
}


static void overtype_char(InputField* field, char ch)
{
    if (field->cursor_pos < field->max_length) {
        field->input_buffer[field->cursor_pos] = ch;

        // Only increment count if we are at the end, effectively adding a new character
        if (field->cursor_pos == field->count) {
            field->count++;
            field->input_buffer[field->count] = '\0'; // Ensure null termination
        }

        // Always advance the cursor
        field->cursor_pos++;
    }
}

void handle_input_char(InputField* field, int ch, bool ins) 
{
    // Check for valid input types and character ranges *before* inserting
    unsigned char upper[255] = {0};
    upper[225] = (unsigned char)193; //Ä
    upper[233] = (unsigned char)201;
    upper[237] = (unsigned char)205;
    upper[243] = (unsigned char)211;
    upper[250] = (unsigned char)218;
    upper[241] = (unsigned char)209;
    switch (field->type) 
    {
      case STRING:
        if (IS_EXTENDED_PRINT((unsigned char)ch)) 
	      {
          if(ins)
          {
            insert_char(field, (unsigned char)ch);
          }
          else
          {
            overtype_char(field, (unsigned char)ch);
          }
        }
        break;
        case INTEGER:
        
        //CHECK ***************************
          if (field->cursor_pos == 0 && ch == '-') 
          {
            if(ins)
            {
              insert_char(field, (char)ch);
            }
            else
            {
              overtype_char(field, (char)ch);
            }
          }
            	if (isdigit(ch)) 
              {
                if(ins)
                {
                  insert_char(field, (char)ch);
                }
                else
                {
                  overtype_char(field, (char)ch);
                }
            	}
            break;
        case FLOAT:
            // Improved FLOAT handling (allows digits, '.', '+', '-')
            if (isdigit(ch) || ch == '.' || ch == '+' || ch == '-') {
                // Add more sophisticated validation for FLOAT here if needed
                // (e.g., only one decimal point, sign only at the beginning).

                // rudimentary validation for float input
                if (ch == '.' && strchr(field->input_buffer, '.') != NULL) {
                  break; // only allow one decimal point
                }
                if ((ch == '+' || ch == '-') && field->cursor_pos != 0 ) {
                   break;
                }
                if(ins)
                {
                  insert_char(field, (char)ch);
                }
                else
                {
                  overtype_char(field, (char)ch);
                }
            }
            break;
        case DATE:
            if (isdigit(ch)) 
            {
                if(ins)
                {
                  insert_char(field, (char)ch);
                }
                else
                {
                  overtype_char(field, (char)ch);
                }
            }
          break;
        case CAP:
          if (IS_EXTENDED_PRINT((unsigned char)ch)) 
	        {
            if(ch >= 'a' && ch <= 'z') ch -= 32;
            else if(upper[ch]) ch = (unsigned char)upper[ch];
            if(ins)
            {
              insert_char(field, (unsigned char)ch);
            }
            else
            {
              overtype_char(field, (unsigned char)ch);
            }
          }
        break;
        default:
            // Handle invalid field type (optional, but good practice)
            break;
    }
}


void handle_backspace(InputField* field) {
 	if (field->cursor_pos > 0) {
        	for (int i = field->cursor_pos; i < field->count; i++) {
            		field->input_buffer[i - 1] = field->input_buffer[i];
        }
        	field->input_buffer[field->count - 1] = '\0';
        	field->count--;
        	field->cursor_pos--;
	}
}

void handle_cursor_left(InputField* field) {
            if (field->cursor_pos > 0) {
        field->cursor_pos--;
    }
}

void handle_cursor_right(InputField* field) {
		if (field->cursor_pos < field->count) {
        field->cursor_pos++;
    }
}

int input_fields_loop(InputField fields[], int num_fields,  void (*background)(void))
{
    int current_field_index = 0;
    bool is_insert_mode = false;
    bool first_field_input = true;
    // Curses should be initialized by caller: initscr(), cbreak(), noecho(), keypad(stdscr, TRUE);
    while(1){

        for (int i = 0; i < num_fields; i++) 
        {
	        if(fields[i].type == DATE) 
          {
		        draw_date_field(&fields[i]);
		      } 
          else 
          {
            draw_input_field(&fields[i]);
		      }
        }

        InputField* active_field = &fields[current_field_index];
        int display_cursor_x_offset;
        if (active_field->type == DATE) 
        {
            display_cursor_x_offset = calculate_date_display_cursor_pos(active_field);
        } 
        else 
        {
            display_cursor_x_offset = active_field->cursor_pos;
        }
        move(active_field->start_y, active_field->start_x + strlen(active_field->prompt) + display_cursor_x_offset);

        refresh();

        int ch = getch();

        if (ch == KEY_ENTER || ch == '\n' || ch == '\r' || ch == PADENTER) 
        {
            if (current_field_index == num_fields - 1) 
            {
                return 0;
            } 
            else 
            {
                first_field_input = true;
                active_field->cursor_pos = 0; // TEST
                current_field_index++;
                // Optional: Reset cursor position for the new active field
                // fields[current_field_index].cursor_pos = fields[current_field_index].count; // e.g., to end
            }
        } 
				else if(ch == 27)
				{
          return 1;
				}
        else if (ch == KEY_UP)
        {
          
            if (current_field_index > 0) 
            {
                first_field_input = true;
                active_field->cursor_pos = 0; // TEST
                current_field_index--;
            }
        } 
        else if (ch == KEY_DOWN) 
        {
            if (current_field_index < num_fields - 1) 
            { // Prevent going past last field
                first_field_input = true;
                active_field->cursor_pos = 0; // TEST
                current_field_index++;
            }
        } 
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) 
        { // Handle common backspace/delete keys
            first_field_input = false;
            handle_backspace(&fields[current_field_index]);
        }
        else if (ch == KEY_LEFT) 
        {
            first_field_input = false;
            handle_cursor_left(&fields[current_field_index]);
        }
        else if (ch == KEY_RIGHT) 
        {
            first_field_input = false;
            handle_cursor_right(&fields[current_field_index]);
        }
        //else if (ch >= 32 && ch <= 126) 
        else if (IS_EXTENDED_PRINT((unsigned char)ch)) 
        { // Standard printable ASCII
            if(first_field_input && (active_field->type == INTEGER || active_field->type == FLOAT) && (isdigit(ch) || ch == '.' || ch == '+' || ch == '-')  )
            {
              for(int i = 0; i < active_field->max_length; i++)
              {
                //Clear active field
                active_field->input_buffer[i] = 0;
              }
              active_field->count = 0;
            }
              handle_input_char(&fields[current_field_index], ch, is_insert_mode);
              if(isdigit(ch) || ch == '.' || ch == '+' || ch == '-') first_field_input = false;
        }
        else if (ch == KEY_IC) 
        {
            is_insert_mode = !is_insert_mode;
        }
    }
}
