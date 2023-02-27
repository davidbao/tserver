/*3.ÊµÊ±¿â´íÎóÂë*/
/*ERR_RTDB_  errno of rtdb --- wj added on 20090218  */

#define  ERR_RTDB_ERROR          -1       /*  error                    */
#define  ERR_RTDB_PARA	       	 -2       /*  bad para_parameter		*/

//access
#define  ERR_RTDB_ACCESSMETH     -3001   /*  access_meth is  error             */
#define  ERR_RTDB_EXIST          -3002   /*  Db name is exist, cannot creat    */
#define  ERR_RTDB_NODB           -3003   /*  Db name not founded               */
#define  ERR_RTDB_INVDBNO        -3004   /*  Db no  is wrong                   */
#define  ERR_RTDB_INVFNAME       -3005   /*  Field name not founded            */
#define  ERR_RTDB_HASOPENED      -3006   /*  The db has been opened	          */
#define  ERR_RTDB_FILE_NOPEN     -3007   /*  The file cannot open              */
#define  ERR_RTDB_READ_FILE      -3008   /*  The file cannot read              */
#define  ERR_RTDB_APP_NO_DB      -3009   /*  This table isn't in current application */
#define  ERR_RTDB_DB_NONE        -3010   /*  The table is deleted or hasn't been created*/

//lock
#define  ERR_RTDB_LOCK           -3111   	/*  The db is locked                  */
#define  ERR_RTDB_UNLOCK         -3112   	/*  The db isnot locked               */
#define  ERR_RTDB_INVUNLOCK      -3113 	/*  locker is not me                  */
#define  ERR_RTDB_NOINDEX        -3114 	/*  havenot index                     */

#define  ERR_RTDB_DIFFSIZE       -3121   	/*  size is different                 */
#define  ERR_RTDB_NOTCHKEY       -3122 	/*          ??                        */
#define  ERR_RTDB_NOCACHE        -3123 	/*  no cache block                    */
#define  ERR_RTDB_NOTE_NEND      -3124 	/*  NOTE must be last field           */

#define  ERR_RTDB_CTRLFULL       -3131 	/*  The db ctrl table is full         */
#define  ERR_RTDB_DBFULL         -3132 	/*  The db is full                    */
#define  ERR_RTDB_EODB           -3133 	/*  End of Database                   */
#define  ERR_RTDB_INVSIZE        -3134  	/*  The size is invalid               */
#define  ERR_RTDB_DBBEGIN        -3135 	/*  Have reach the begin of db        */
#define  ERR_RTDB_DBEND          -3136 	/*  Have reach the end of db          */

#define	 ERR_RTDB_INVCCOMM       -3137 	/*  Invalid comparition command       */
#define  ERR_RTDB_TMOPEN         -3138 	/*  Too many open                     */
#define  ERR_RTDB_EMPTY			-3139     /*  table is empty 					*/

////key
#define  ERR_RTDB_KEYNOTFIND     -3241 	/*  keyword not founded               */
#define  ERR_RTDB_MULTIKEY       -3242 	/*  Multi-keywords                    */
#define  ERR_RTDB_INVKEY         -3243 	/*  The keyword is invalid            */
#define  ERR_RTDB_MKEYFIELD      -3244 	/*  The keyword field can be only one */
#define  ERR_RTDB_KEYFLEN        -3245 	/*  The keyword length must be 4 times*/
#define  ERR_RTDB_DBTYPE         -3246 	/*  Invalid db type                   */
#define  ERR_RTDB_KEYTYPE        -3247 	/*  Invalid keyword type              */
#define  ERR_RTDB_PASSWD         -3248 	/*  Invalid passwd                    */

////table
#define  ERR_RTDB_TABLE_NO       -3300  	/*  no match table or no*/
#define  ERR_RTDB_TABLE_STA		 -3301  	/*  table status is abnormal */

////record
#define  ERR_RTDB_EXISTREC       -3451 	/*  Record existed, cannot write      */
#define  ERR_RTDB_NEXISTREC      -3452 	/*  Record not existed, cannot modify */
#define  ERR_RTDB_SUPMAX         -3453 	/*  record value too bigger           */
#define  ERR_RTDB_BELMIN         -3454 	/*  record value too smaller          */
#define  ERR_RTDB_RECSIZE        -3455 	/*  record size wrong                 */
#define  ERR_RTDB_NOTERASE       -3456 	/*  record not erased                 */
#define  ERR_RTDB_RECERASE       -3457 	/*  The record is erased              */
#define  ERR_RTDB_POINTER        -3458   	/*  record_pointer error              */
#define  ERR_RTDB_CONT_ERR       -3459 	/*  condition express error           */

////field
#define  ERR_RTDB_DATATYPE       -3561 	/*  field data type is wrong          */
#define  ERR_RTDB_DIFFTYPE       -3562 	/*  type is different                 */
#define  ERR_RTDB_KEYFIELD       -3563 	/*  The field is keyword              */
#define  ERR_RTDB_BIGFIELDNO     -3564 	/*  The field no is too big           */
#define  ERR_RTDB_DLT_WRITE      -3565 	/*  The direct link hash cannot write */
#define  ERR_RTDB_INVFIELDNO     -3566   	/*  field_no is wrong		          */
#define  ERR_RTDB_NOTFIND        -3567 	/*  Not find the value                */
#define  ERR_RTDB_FIELD_NO       -3568  	/*  no match field name or no          */
#define  ERR_RTDB_FIELD_SIZE     -3569  	/*  field length wrong                 */
#define  ERR_RTDB_NONE_FIELD     -3570  	/*  this table has no field            */


//operation
#define  ERR_RTDB_INVCOMM        -3601   /*  Invalid db operater command       */
#define  ERR_RTDB_NPERMIT        -3602 	/*  The OP is not permit              */

#define  ERR_RTDB_CONTEXT        -3620   /*  no context or application         */
#define  ERR_RTDB_APP_UNLOAD     -3621   /*  application isn't loaded          */


#define ERR_RTDB_SQL_COMMAND     -3631   /*select error*/
#define ERR_RTDB_SQL_FORM        -3632   /*form error*/
#define ERR_RTDB_SQL_WHERE       -3633   /*where condition error*/
#define ERR_RTDB_SQL_ORDER       -3634   /*order by error*/

//memory
#define  ERR_RTDB_MEMORY         -3700   /*  Cannot malloc or calloc memory    */
#define  ERR_RTDB_BUFFFULL       -3701 	 /*  Buffer is full                    */
#define  ERR_RTDB_MEM_NOALLOC    -3702   /*  The memory cannot allocation      */

#define  ERR_RTDB_MAP            -3710   /*  map file error                   */
#define  ERR_RTDB_SHM		-3711    /*  get shm error				*/

#define  ERR_RTDB_OFLOW_MALLOC	-3712	/*  malloc error in overflow area */
#define  ERR_RTDB_OFLOW_FREE	-3713	/*  free error in overflow area */
#define  ERR_RTDB_STG_HASH_SAME_HASH_NUM -3715 /*error when INDEX_STORAGE_HASH_REC_HEAD::same_hash_num<0 */
#define  ERR_RTDB_WRONG_MEM	-3716  /* shmget or mmap a wrong memory */
#define  ERR_RTDB_INDEX_MEM_ALLOC_TYPE -3717  /*wrong index memory allocation type */
//#define  ERR_RTDB_PTABFULL  -10    	/*  process tab is full               */

//sql
#define ERR_RTDB_SQL			 -3720   /*  sql syntax error	*/

#define  ERR_RTDB_DBNAMENOTF     -3800     /*  db_name not founded                */
#define  ERR_RTDB_FACNAMENOTF    -3801     /*  fac_name not founded	              */
#define  ERR_RTDB_VALNAMENOTF    -3802     /*  val_name not founded	              */
#define  ERR_RTDB_FIELDNAMENOTF  -3803     /*  field_name not founded	          */
#define  ERR_RTDB_VIEWFIELD      -3901     /*  view get fieldnum over error       */
#define  ERR_RTDB_SYBASE	     -3902     /*  modify sybase error                */
#define  ERR_RTDB_INDEXERR	     -3903     /*  index number error                 */
#define  ERR_RTDB_MODIFYSIZE     -3904     /*  modify bufsize error               */
#define  ERR_RTDB_API_INVCOMM       -3905  	/*  Invalid db operater command       */
#define  ERR_RTDB_API_NOAPP         -3906  	/*  App name not founded              */
#define  ERR_RTDB_API_NODB          -3907  	/*  Db  name not founded              */
#define  ERR_RTDB_API_INVDBNO       -3908  	/*  Db no  is wrong                   */
#define  ERR_RTDB_API_NODEOFF       -3909  	/*  node  is offline                  */
#define  ERR_RTDB_API_ERRINIT       -3910  	/*  odbms init error                  */
#define  ERR_RTDB_API_NEWERR	    -3911  	/*  new space error                   */
#define  ERR_RTDB_API_DATAERR       -3912  	/*  data tye error                    */
#define  ERR_RTDB_NOSPACE   		-3915  	/*  no space left in disk             */

/* end of ERR_RTDB_    */
