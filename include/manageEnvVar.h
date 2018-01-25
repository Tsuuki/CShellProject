/**
 * List all the existing environment variables 
 */
void listEnvVar();

/**
 * Add a variable to the environment
 * \param char* name name of the new variable
 * \param char* value value of the new variable
 */
void addEnvVar(char*, char*);

/**
 * Get an environment variable
 * \param char* name name of the variable to get
 * \return char* value value of the variable
 */
char* getEnvVar(char*);

/**
 * Delete the environment variable
 * \param char* name name of the variable to delete
 */
void delEnvVar(char*);