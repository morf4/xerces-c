/*
* The Apache Software License, Version 1.1
* 
* Copyright (c) 1999 The Apache Software Foundation.  All rights 
* reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 
* 3. The end-user documentation included with the redistribution,
*    if any, must include the following acknowledgment:  
*       "This product includes software developed by the
*        Apache Software Foundation (http://www.apache.org/)."
*    Alternately, this acknowledgment may appear in the software itself,
*    if and wherever such third-party acknowledgments normally appear.
* 
* 4. The names "Xerces" and "Apache Software Foundation" must
*    not be used to endorse or promote products derived from this
*    software without prior written permission. For written 
*    permission, please contact apache\@apache.org.
* 
* 5. Products derived from this software may not be called "Apache",
*    nor may "Apache" appear in their name, without prior written
*    permission of the Apache Software Foundation.
* 
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
* ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
* USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
* ====================================================================
* 
* This software consists of voluntary contributions made by many
* individuals on behalf of the Apache Software Foundation, and was
* originally based on software copyright (c) 1999, International
* Business Machines, Inc., http://www.ibm.com .  For more information
* on the Apache Software Foundation, please see
* <http://www.apache.org/>.
*/

/**
* $Log$
* Revision 1.3  1999/12/03 00:11:23  andyh
* Added DOMString.clone() to node parameters in and out of the DOM,
* where they had been missed.
*
* DOMString::rawBuffer, removed incorrect assumptions about it
* being null terminated.
*
* Revision 1.2  1999/11/30 21:16:25  roddey
* Changes to add the transcode() method to DOMString, which returns a transcoded
* version (to local code page) of the DOM string contents. And I changed all of the
* exception 'throw by pointer' to 'throw by value' style.
*
* Revision 1.1.1.1  1999/11/09 01:09:13  twl
* Initial checkin
*
* Revision 1.3  1999/11/08 20:44:29  rahul
* Swat for adding in Product name and CVS comment log variable.
*
*/


#include "NodeImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Node.hpp"
#include "DOMString.hpp"
#include "DocumentImpl.hpp"
#include "stdio.h"
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>

NodeImpl::NodeImpl(DocumentImpl *ownerDoc,
                   const DOMString &nam,  short nTyp,
                   bool isLeafNod, const DOMString &initValue)
{
    // Do we want to add isLeafNode to this? How about initial value?
    this->ownerDocument=ownerDoc;
    this->namespaceURI=null;	//DOM Level 2
    this->prefix=null;			//DOM Level 2
    this->localName=null;		//DOM Level 2
    this->name=nam.clone();
    this->nType=nTyp;
    this->isLeafNode=isLeafNod;
    this->value=initValue.clone();
    
    this->changes = 0;
    this->userData = null;
    this->readOnly = false;
    this->owned    = false;
    this->firstChild = null;
    this->lastChild  = null;
    this->previousSibling  = null;
    this->nextSibling  = null;
    this->parentNode  = null;
    
    this->nodeRefCount = 0;
    NodeImpl::gLiveNodeImpls++; 
    NodeImpl::gTotalNodeImpls++;
};  


//Introduced in DOM Level 2
NodeImpl::NodeImpl(DocumentImpl *ownerDoc,
                   const DOMString &namespaceURI, const DOMString &qualifiedName, short nTyp,
                   bool isLeafNod, const DOMString &initValue)
{
    // Do we want to add isLeafNode to this? How about initial value?
    this->ownerDocument=ownerDoc;
    this->name=qualifiedName.clone();
    
    this->namespaceURI=namespaceURI.clone();

    // Look in the qualified name parameter for a colon.
    //  If found, break out the prefix and local name parts.
    //    (bug:  check for well formed names.)
    XMLCh  *qNameP   = this->name.rawBuffer();
    int    qNameLen = this->name.length();
    int index;
    for (index=0; index<qNameLen; index++)
    {
        if (qNameP[index] == chColon)
        {
            this->prefix = this->name.substringData(0, index);
            int len = this->name.length() - index - 1;
            this->localName = this->name.substringData(index+1, len);
            break;
        }
    }

    if (this->prefix == null)
    {
        // The search for a colon, above, in the qualified name failed.
        // the localname is the whole name, and there is no prefix part.
        this->localName = this->name;
    }

    this->nType=nTyp;
    this->isLeafNode=isLeafNod;
    this->value=initValue.clone();
    
    this->changes = 0;
    this->userData = null;
    this->readOnly = false;
    this->owned    = false;
    this->firstChild = null;
    this->lastChild  = null;
    this->previousSibling  = null;
    this->nextSibling  = null;
    this->parentNode  = null;
    
    this->nodeRefCount = 0;
    NodeImpl::gLiveNodeImpls++; 
    NodeImpl::gTotalNodeImpls++;
};  


NodeImpl::NodeImpl(const NodeImpl &other, bool deep) {
    this->nType = other.nType;
    this->namespaceURI = other.namespaceURI.clone();	//DOM Level 2
    this->prefix = other.prefix.clone();                //DOM Level 2
    this->localName = other.localName.clone();          //DOM Level 2
    this->name  = other.name.clone();
    this->value = other.value.clone();
    this->isLeafNode = other.isLeafNode;
    this->readOnly = false;
    this->owned = false;
    this->ownerDocument = other.ownerDocument;
    this->userData = other.userData;
    this->changes = 0;
    
    this->nodeRefCount = 0;
    NodeImpl::gLiveNodeImpls++; 
    NodeImpl::gTotalNodeImpls++;
    
    
    // Need to break the association w/ original kids
    this->previousSibling = null;
    this->nextSibling = null;
    this->parentNode = null;
    this->firstChild = null;
    this->lastChild = null;
    
    // Then, if deep, clone the kids too.
    if (deep)
    {
        for (NodeImpl *mykid = other.firstChild; 
        mykid != null; 
        mykid = mykid->nextSibling)
            this->appendChild(mykid->cloneNode(true));
    }
    
};


int  NodeImpl::gLiveNodeImpls = 0;         // Counters for debug & tuning.
int  NodeImpl::gTotalNodeImpls= 0;



NodeImpl::~NodeImpl() {
    NodeImpl::gLiveNodeImpls--;
};


// Dynamic Cast substitute functions
bool NodeImpl::isAttrImpl()              {return false;};
bool NodeImpl::isCDATASectionImpl()      {return false;};
bool NodeImpl::isDocumentFragmentImpl()  {return false;};
bool NodeImpl::isDocumentImpl()          {return false;};
bool NodeImpl::isDocumentTypeImpl()      {return false;};
bool NodeImpl::isElementImpl()           {return false;};
bool NodeImpl::isEntityReference()       {return false;};
bool NodeImpl::isTextImpl()              {return false;};


NodeImpl * NodeImpl::appendChild(NodeImpl *newChild)      
{
    return insertBefore(newChild, null);
};



void NodeImpl::changed()
{
    for (NodeImpl *n=this; n != null; n=n->parentNode)
        ++n->changes;
};  


NodeImpl * NodeImpl::cloneNode(bool deep) {
    NodeImpl *newnode;
    newnode = new NodeImpl(*this, deep);
    return newnode;
};


//  NodeImpl::deleteIf is called when a node's reference count goes
//  to 0.  It is separate function from removeRef because removeRef
//  is likely to be in-lined.
//
//  See comments at RefCountedImpl::removeRef().
//
void NodeImpl::deleteIf(NodeImpl *thisNode)
{
    if (thisNode == 0)
        return;
    
    if (thisNode->parentNode != 0 || thisNode->owned)
        return;
    
    // Delete this node.  There should be no siblings, as the DOM
    //  supports no node operations that would detach a node from
    //  its parent while retaining siblings.
    //  The target node may have children, in which case they must
    //  be removed from this node before deleting this node.
    thisNode->readOnly = false;   // removeChild requires node not be readonly.
    NodeImpl *theNextChild;
    for (NodeImpl *child = thisNode->firstChild; child != 0; child=theNextChild)
    {
        theNextChild = child->nextSibling;
        thisNode->removeChild(child);
        if (child->nodeRefCount == 0)
            deleteIf(child);
    }
    delete thisNode;
};



NamedNodeMapImpl * NodeImpl::getAttributes() {
    return null; // overridden in ElementImpl
};


NodeListImpl *NodeImpl::getChildNodes() {
    return this;
};



NodeImpl * NodeImpl::getFirstChild() {
    return firstChild;
}; 


NodeImpl * NodeImpl::getLastChild()
{
    return lastChild;
}; 


int NodeImpl::getLength() {
    int count = 0;
    NodeImpl *node = firstChild;
    while(node != null)
    {
        ++count;
        node = node->nextSibling;
    }
    return count;
};


NodeImpl * NodeImpl::getNextSibling() {
    return nextSibling;
}; 



DOMString NodeImpl::getNodeName() {
    return name;
};


short NodeImpl::getNodeType()
{
    return nType;
};



DOMString NodeImpl::getNodeValue()
{
    return value;
};


DocumentImpl *NodeImpl::getOwnerDocument()
{
    return ownerDocument;
};  


NodeImpl * NodeImpl::getParentNode()
{
    return parentNode;
};  


NodeImpl*  NodeImpl::getPreviousSibling()
{
    return previousSibling;
}; 


void *NodeImpl::getUserData()
{
    return userData;
};  


bool NodeImpl::hasChildNodes()
{ 
    return firstChild!=null;
}; 



NodeImpl *NodeImpl::insertBefore(NodeImpl *newChild, NodeImpl *refChild) {
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    if(  !(newChild->getOwnerDocument() == ownerDocument   ||
        // SPECIAL-CASE: Document has no owner, but may be the owner.
        ( this->isDocumentImpl() &&
        newChild->getOwnerDocument() == (DocumentImpl *)this ) 
        ) )
        throw DOM_DOMException(DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    
    // Convert to internal type, to avoid repeated casting  
    //   (left over from the original Java.  Meaningless in this version.)
    NodeImpl * newInternal= newChild;
    
    // Prevent cycles in the tree
    bool treeSafe=true;
    for(NodeImpl *a=this->parentNode;treeSafe && a!=null;a=a->parentNode)
        treeSafe=(newInternal!=a);
    if(!treeSafe)
        throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
    
    // refChild must in fact be a child of this node (or null)
    if(refChild!=null && refChild->parentNode != this)
        throw DOM_DOMException(DOM_DOMException::NOT_FOUND_ERR,null);
    
    if (newInternal->isDocumentFragmentImpl())
    {
        // SLOW BUT SAFE: We could insert the whole subtree without
        // juggling so many next/previous pointers. (Wipe out the
        // parent's child-list, patch the parent pointers, set the
        // ends of the list.) But we know some subclasses have special-
        // case behavior they add to insertBefore(), so we don't risk it.
        // This approch also takes fewer bytecodes.
        
        // NOTE: If one of the children is not a legal child of this
        // node, throw HIERARCHY_REQUEST_ERR before _any_ of the children
        // have been transferred. (Alternative behaviors would be to
        // reparent up to the first failure point or reparent all those
        // which are acceptable to the target node, neither of which is
        // as robust. PR-DOM-0818 isn't entirely clear on which it
        // recommends?????
        
        // No need to check kids for right-document; if they weren't,
        // they wouldn't be kids of that DocFrag.
        for(NodeImpl *kid=newInternal->getFirstChild(); // Prescan
        kid!=null;
        kid=kid->getNextSibling())
        {
            if(!isKidOK(this,kid))
                throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
        }                       
        while(newInternal->hasChildNodes())     // Move
            insertBefore(newInternal->getFirstChild(),refChild);
    }
    
    else if(!isKidOK(this, newInternal))
        throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
    
    else
    {
        NodeImpl *oldparent=newInternal->parentNode;
        if(oldparent!=null)
            oldparent->removeChild(newInternal);
        
        NodeImpl *prev;
        // Find the node we're inserting after, if any (null if
        // inserting to head of list)
        prev= (refChild==null)
            ? lastChild
            : refChild->previousSibling;
        
        // Attach up
        newInternal->parentNode=this;
        
        // Attach after
        newInternal->previousSibling=prev;
        if(prev==null)
            firstChild=newInternal;
        else
            prev->nextSibling=newInternal;
        
        // Attach before
        newInternal->nextSibling = refChild;
        if(refChild==null)
            lastChild=newInternal;
        else
            refChild->previousSibling=newInternal;
    }
    changed();
    
    return newInternal;
};


/** Uses the kidOK lookup table to check whether the proposed
tree structure is legal.

  ????? It feels like there must be a more efficient solution,
  but for the life of me I can't think what it would be.
  */
  bool NodeImpl::isKidOK(NodeImpl *parent, NodeImpl *child)
  {
      static int kidOK[13];
      
      if (kidOK[DOM_Node::DOCUMENT_NODE] == 0)
      {
          kidOK[DOM_Node::DOCUMENT_NODE] = 
              1 << DOM_Node::ELEMENT_NODE |
              1 << DOM_Node::PROCESSING_INSTRUCTION_NODE | 
              1 << DOM_Node::COMMENT_NODE | 
              1 << DOM_Node::DOCUMENT_TYPE_NODE;
          
          kidOK[DOM_Node::DOCUMENT_FRAGMENT_NODE] = 
              kidOK[DOM_Node::ENTITY_NODE] = 
              kidOK[DOM_Node::ENTITY_REFERENCE_NODE] = 
              kidOK[DOM_Node::ELEMENT_NODE] = 
              1 << DOM_Node::ELEMENT_NODE |
              1 << DOM_Node::PROCESSING_INSTRUCTION_NODE | 
              1 << DOM_Node::COMMENT_NODE |
              1 << DOM_Node::TEXT_NODE |
              1 << DOM_Node::CDATA_SECTION_NODE |
              1 << DOM_Node::ENTITY_REFERENCE_NODE;
          
          kidOK[DOM_Node::DOCUMENT_TYPE_NODE] = 
              1 << DOM_Node::NOTATION_NODE |
              1 << DOM_Node::ENTITY_NODE;
          
          kidOK[DOM_Node::ATTRIBUTE_NODE] = 
              1 << DOM_Node::TEXT_NODE |
              1 << DOM_Node::ENTITY_REFERENCE_NODE;
          
          kidOK[DOM_Node::PROCESSING_INSTRUCTION_NODE] = 
              kidOK[DOM_Node::COMMENT_NODE] = 
              kidOK[DOM_Node::TEXT_NODE] = 
              kidOK[DOM_Node::CDATA_SECTION_NODE] = 
              kidOK[DOM_Node::NOTATION_NODE] = 
              0;
      };
      int p=parent->getNodeType();
      int ch = child->getNodeType();
      return (kidOK[p] & 1<<ch) != 0;
  };
  
  
  
  NodeImpl *NodeImpl::item(int index) {
      NodeImpl *node = firstChild;
      for(int i=0; i<index && node!=null; ++i)
          node = node->nextSibling;
      return node;
  };
  
  
  void NodeImpl::referenced()
  {
      RefCountedImpl::addRef(this->ownerDocument);
  };
  
  
  
  NodeImpl *NodeImpl::removeChild(NodeImpl *oldChild) 
  {
      if (readOnly)
          throw DOM_DOMException(
          DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
      
      if (oldChild != null && oldChild->parentNode != this)
          throw DOM_DOMException(DOM_DOMException::NOT_FOUND_ERR, null);
      
      // Patch tree past oldChild
      NodeImpl *prev = oldChild->previousSibling;
      NodeImpl *next = oldChild->nextSibling;
      if (prev != null)
          prev->nextSibling = next;
      else
          firstChild = next;
      if (next != null)
          next->previousSibling = prev;
      else
          lastChild = prev;
      
      // Remove oldChild's references to tree
      oldChild->parentNode = null;
      oldChild->nextSibling = null;
      oldChild->previousSibling = null;
      changed();
      return oldChild;
  };
  
  
  //
  //    unreferenced  will be called whenever the refernce count on
  //            this node goes from 1 to 0.  This node will only be
  //            directly deleted here  (by deleteIf) if it is outside 
  //            of the document tree.  
  //
  void NodeImpl::unreferenced()
  {
      DocumentImpl *doc = this->ownerDocument;
      deleteIf(this);       // This gets nodes outside of the document -
      //   deleteIf() deletes only if the parent
      //   node is null.  
      
      // If this was the last external reference within the document,
      //    the entire document will be deleted as well.
      RefCountedImpl::removeRef(doc);
  };
  
  
  
  NodeImpl *NodeImpl::replaceChild(NodeImpl *newChild, NodeImpl *oldChild)
  {
  /********* insertBefore should already do this test. ****************
  if (newChild.getOwnerDocument() != ownerDocument
  // SPECIAL-CASE: Document has no Owner.
  &&
  this instanceof Document && 
  newChild.getOwnerDocument() != (Document) this)
  {
  throw DOMExceptionImpl(DOMException.WRONG_DOCUMENT_ERR, null);
  }
      *********************************************************************/
      insertBefore(newChild, oldChild);
      // changed() already done.
      return removeChild(oldChild);
  };
  
  
  
  
  void NodeImpl::setNodeValue(const DOMString &val)
  {
      if (readOnly)
          throw DOM_DOMException(
          DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
      
      // Default behavior, overridden in some subclasses
      this->value = val.clone();
  };
  
  
  
  
  void NodeImpl::setReadOnly(bool readOnl, bool deep)
  {
      this->readOnly = readOnl;
      
      if (deep)
          // Recursively set kids
          for (NodeImpl *mykid = firstChild; 
          mykid != null; 
          mykid = mykid->nextSibling)
              if(! (mykid->isEntityReference()))
                  mykid->setReadOnly(readOnl,true);
  };
  
  
  
  void NodeImpl::setUserData(void * val)
  {
      userData = val;
  };  
  
  
  
  DOMString NodeImpl::toString()
  {
      return DOMString("[")+getNodeName()+": "+getNodeValue()+"]";
      // return getNodeName();
  };    
  
  //Introduced in DOM Level 2
  
  bool NodeImpl::supports(const DOMString &feature, const DOMString &version)
  {
      return false;   //must be overriden by each subclass
  }
  
  DOMString NodeImpl::getNamespaceURI()
  {
      return namespaceURI;
  }
  
  DOMString NodeImpl::getPrefix()
  {
      return prefix;
  }
  
  DOMString NodeImpl::getLocalName()
  {
      return localName;
  }
  
  void NodeImpl::setPrefix(const DOMString &prefix)
  {
      if (readOnly)
          throw DOM_DOMException(
          DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
      if (isAttrImpl() || isElementImpl()) {
          name = this -> prefix = prefix;
          name = name + chColon + localName;    //nodeName is changed too
          //what to do if namespaceURI is null?
      }
      
  }
