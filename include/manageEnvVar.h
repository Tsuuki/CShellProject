/**
 * Print all the existing environment variables in the shell
 * 
 * \return void
 */
void printEnvVar();

/**
 * Add a variable to the environment
 * 
 * \param char* string containing the name and the value of the var
 * \return void
 */
void addEnvVar(char* str);

/**
 * Get the value of environment variable passed in parameter
 * 
 * \param char* name of the variable to get
 * \return char* value of the variable
 */
char* getEnvVar(char* name);

/**
 * Delete the environment variable
 * 
 * \param char* name of the variable to delete
 * \return void
 */
void delEnvVar(char* name);