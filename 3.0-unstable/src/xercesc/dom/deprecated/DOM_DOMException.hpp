/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 */

#ifndef DOM_DOMException_HEADER_GUARD_
#define DOM_DOMException_HEADER_GUARD_

#include <xercesc/util/XercesDefs.hpp>
#include "DOMString.hpp"

XERCES_CPP_NAMESPACE_BEGIN


/**
  * Encapsulate a general DOM error or warning.
  *
  * <p> The DOM will create and throw an instance of DOMException
  * when an error condition is detected.  Exceptions can occur
  * when an application directly manipulates the DOM document
  * tree that is produced by the parser, or when a document tree
  * is created from scratch using the DOM API.  DOM exceptions will
  * not be generated by the parser while constructing a document
  * tree from an XML source document.
  *
  * <p>Unlike the other classes in the C++ DOM API, DOM_DOMException
  * is NOT a reference to an underlying implementation class, and
  * does not provide automatic memory management.  Code that catches
  * a DOM exception is responsible for deleting it, or otherwise
  * arranging for its disposal.
  *
  */
class DEPRECATED_DOM_EXPORT DOM_DOMException  {
public:
    /** @name Enumerators for DOM Exceptions */
    //@{
        enum ExceptionCode {
                INDEX_SIZE_ERR       = 1,
                DOMSTRING_SIZE_ERR   = 2,
                HIERARCHY_REQUEST_ERR = 3,
                WRONG_DOCUMENT_ERR   = 4,
                INVALID_CHARACTER_ERR = 5,
                NO_DATA_ALLOWED_ERR  = 6,
                NO_MODIFICATION_ALLOWED_ERR = 7,
                NOT_FOUND_ERR        = 8,
                NOT_SUPPORTED_ERR    = 9,
                INUSE_ATTRIBUTE_ERR  = 10,
                INVALID_STATE_ERR    = 11,
	       	SYNTAX_ERR	     = 12,
        	INVALID_MODIFICATION_ERR    = 13,
        	NAMESPACE_ERR	     = 14,
        	INVALID_ACCESS_ERR   = 15
        };
    //@}
public:
    /** @name Constructors and assignment operator */
    //@{
    /**
      * Default constructor for DOM_DOMException.
      *
      */
    DOM_DOMException();

    /**
      * Constructor which takes an error code and a message.
      *
      * @param code The error code which indicates the exception
      * @param message The string containing the error message
      */
    DOM_DOMException(short code, const DOMString &message);

    /**
      * Copy constructor.
      *
      * @param other The object to be copied.
      */
    DOM_DOMException(const DOM_DOMException &other);

    //@}
    /** @name Destructor. */
    //@{
	 /**
	  * Destructor for DOM_DOMException.  Applications are responsible
      * for deleting DOM_Exception objects that they catch after they
      * have completed their exception processing.
	  *
	  */
    virtual ~DOM_DOMException();
    //@}

    /** @name Public variables. */
     //@{
	 /**
	  * A code value, from the set defined by the ExceptionCode enum,
      * indicating the type of error that occured.
	  */
   ExceptionCode   code;

	 /**
	  * A string value.  Applications may use this field to hold an error
      *  message.  The field value is not set by the DOM implementation,
      *  meaning that the string will be empty when an exception is first
      *  thrown.
	  */
    DOMString msg;
    //@}

};

XERCES_CPP_NAMESPACE_END

#endif

