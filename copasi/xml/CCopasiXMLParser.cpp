/**
 * CCopasiXMLParser class.
 * This class parses a Copasi XML file.
 *
 * Created for Copasi by Stefan Hoops 2003
 * Copyright Stefan Hoops
 */

#include "copasi.h"

#include "CCopasiXMLParser.h"
#include "CCopasiXMLInterface.h"
#include "utilities/CCopasiMessage.h"
#include "function/CFunction.h"

CCopasiXMLParser::ModelElement::ModelElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common)
{}

CCopasiXMLParser::ModelElement::~ModelElement() {}

void CCopasiXMLParser::ModelElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{}

void CCopasiXMLParser::ModelElement::end(const XML_Char *pszName) {}

CCopasiXMLParser::ListOfTasksElement::ListOfTasksElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common)
{}

CCopasiXMLParser::ListOfTasksElement::~ListOfTasksElement() {}

void CCopasiXMLParser::ListOfTasksElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{}

void CCopasiXMLParser::ListOfTasksElement::end(const XML_Char *pszName) {}

CCopasiXMLParser::ListOfReportsElement::ListOfReportsElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common)
{}

CCopasiXMLParser::ListOfReportsElement::~ListOfReportsElement() {}

void CCopasiXMLParser::ListOfReportsElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{}

void CCopasiXMLParser::ListOfReportsElement::end(const XML_Char *pszName) {}

CCopasiXMLParser::COPASIElement::COPASIElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common)
{}

CCopasiXMLParser::COPASIElement::~COPASIElement() {}

void CCopasiXMLParser::COPASIElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{
  mCurrentElement++; /* We should always be on the next element */

  switch (mCurrentElement)
    {
    case COPASI:
      if (strcmp(pszName, "COPASI")) fatalError();
      mCurrentElement = -1;
      return;

    case ListOfFunctions:
      if (strcmp(pszName, "ListOfFunctions")) fatalError();
      mpCurrentHandler = new ListOfFunctionsElement(mCommon);
      break;

    case Model:
      if (strcmp(pszName, "Model")) fatalError();
      mpCurrentHandler = new ModelElement(mCommon);
      break;

    case ListOfTasks:
      if (strcmp(pszName, "ListOfTasks")) fatalError();
      mpCurrentHandler = new ListOfTasksElement(mCommon);
      break;

    case ListOfReports:
      if (strcmp(pszName, "ListOfReports")) fatalError();
      mpCurrentHandler = new ListOfReportsElement(mCommon);
      break;

    default:
      fatalError();
      break;
    }

  mStack.push(mpCurrentHandler);
  mpCurrentHandler->start(pszName, papszAttrs);

  return;
}

void CCopasiXMLParser::COPASIElement::end(const XML_Char *pszName)
{
  mStack.pop();
  pdelete(mpCurrentHandler);
}

CCopasiXMLParser::ListOfFunctionsElement::ListOfFunctionsElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common)
{}

CCopasiXMLParser::ListOfFunctionsElement::~ListOfFunctionsElement()
{
  pdelete(mpCurrentHandler);
}

void CCopasiXMLParser::ListOfFunctionsElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{
  mCurrentElement++; /* We should always be on the next element */

  switch (mCurrentElement)
    {
    case ListOfFunctions:
      if (strcmp(pszName, "ListOfFunctions")) fatalError();
      if (!mCommon.pFunctionList)
        mCommon.pFunctionList = new CCopasiVectorN< CFunction >;
      break;

    case Function:
      if (strcmp(pszName, "Function")) fatalError();

      /* If we do not have a function element handler we create one. */
      if (!mpCurrentHandler)
        mpCurrentHandler = new FunctionElement(mCommon);

      /* Push the Function element handler on the stack and call it. */
      mStack.push(mpCurrentHandler);
      mpCurrentHandler->start(pszName, papszAttrs);
      break;

    default:
      fatalError();
      break;
    }

  return;
}

void CCopasiXMLParser::ListOfFunctionsElement::end(const XML_Char *pszName)
{
  mStack.pop();

  switch (mCurrentElement)
    {
    case ListOfFunctions:
      if (strcmp(pszName, "ListOfFunctions")) fatalError();

      /* Tell the parent element we are done. */
      mStack.top()->end(pszName);
      break;

    case Function:
      if (strcmp(pszName, "Function")) fatalError();
      break;

    default:
      fatalError();
      break;
    }

  mCurrentElement--;

  return;
}

CCopasiXMLParser::FunctionElement::FunctionElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common),
    mpMathMLElement(NULL),
    mpListOfParameterDescriptionsElement(NULL)
{}

CCopasiXMLParser::FunctionElement::~FunctionElement()
{
  pdelete(mpMathMLElement);
  pdelete(mpListOfParameterDescriptionsElement);
}

void CCopasiXMLParser::FunctionElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{
  unsigned C_INT32 i = 1; /* Function of Type Base are never instantiated */
  const char * Key;
  const char * Type;
  const char * Name;

  mCurrentElement++; /* We should always be on the next element */

  switch (mCurrentElement)
    {
    case Function:
      if (strcmp(pszName, "Function")) fatalError();

      Key = CCopasiXMLInterface::getAttributeValue("key", papszAttrs);
      Name = CCopasiXMLInterface::getAttributeValue("name", papszAttrs);
      Type = CCopasiXMLInterface::getAttributeValue("type", papszAttrs);

      while (CFunction::XMLType[i] != "" &&
             CFunction::XMLType[i] == Type) i++;
      if (CFunction::XMLType[i] == "") fatalError();

      mCommon.pFunction = CFunction::createFunction((enum CFunction::Type) i);
      mCommon.pFunction->setName(Name);

      /* We have a new function and add it to the list */
      mCommon.pFunctionList->add(mCommon.pFunction, true);
      mCommon.FunctionKeyMap[Key] = & mCommon.pFunction->getKey();

      break;

    case MathML:
      if (strcmp(pszName, "MathML")) fatalError();

      /* If we do not have a MathML element handler we create one. */
      if (!mpMathMLElement)
        mpMathMLElement = new MathMLElement(mCommon);

      /* Push the MathML element handler on the stack and call it. */
      mpCurrentHandler = mpMathMLElement;
      mStack.push(mpCurrentHandler);
      mpCurrentHandler->start(pszName, papszAttrs);
      break;

    case ListOfParameterDescriptions:
      if (strcmp(pszName, "ListOfParameterDescriptions")) fatalError();

      /* If we do not have a ListOfParameterDescriptions element handler we create one. */
      if (!mpListOfParameterDescriptionsElement)
        mpListOfParameterDescriptionsElement = new ListOfParameterDescriptionsElement(mCommon);

      /* Push the ListOfParameterDescriptions element handler on the stack and call it. */
      mpCurrentHandler = mpListOfParameterDescriptionsElement;
      mStack.push(mpCurrentHandler);
      mpCurrentHandler->start(pszName, papszAttrs);
      break;

    default:
      fatalError();
      break;
    }

  return;
}

void CCopasiXMLParser::FunctionElement::end(const XML_Char *pszName)
{
  mStack.pop();

  switch (mCurrentElement)
    {
    case Function:
      if (strcmp(pszName, "Function")) fatalError();
      mCurrentElement = -1;

      /* Tell the parent element we are done. */
      mStack.top()->end(pszName);
      break;

    case MathML:
      if (strcmp(pszName, "MathML")) fatalError();
      break;

    case ListOfParameterDescriptions:
      if (strcmp(pszName, "ListOfParameterDescriptions")) fatalError();
      break;

    default:
      fatalError();
      break;
    }
}

CCopasiXMLParser::MathMLElement::MathMLElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common)
{}

CCopasiXMLParser::MathMLElement::~MathMLElement()
{
  pdelete(mpCurrentHandler);
}

void CCopasiXMLParser::MathMLElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{
  mCurrentElement++; /* We should always be on the next element */

  switch (mCurrentElement)
    {
    case MathML:
      if (strcmp(pszName, "MathML")) fatalError();
      break;

    case Text:
      if (strcmp(pszName, "Text")) fatalError();

      /* If we do not have a function element handler we create one. */
      if (!mpCurrentHandler)
        mpCurrentHandler = new TextElement(mCommon);

      /* Push the Function element handler on the stack and call it. */
      mStack.push(mpCurrentHandler);
      mpCurrentHandler->start(pszName, papszAttrs);
      break;

    default:
      fatalError();
      break;
    }

  return;
}

void CCopasiXMLParser::MathMLElement::end(const XML_Char *pszName)
{
  mStack.pop();

  switch (mCurrentElement)
    {
    case MathML:
      if (strcmp(pszName, "MathML")) fatalError();
      mCurrentElement = -1;

      /* Tell the parent element we are done. */
      mStack.top()->end(pszName);
      break;

    case Text:
      if (strcmp(pszName, "Text")) fatalError();
      break;

    default:
      fatalError();
      break;
    }
}

CCopasiXMLParser::TextElement::TextElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common)
{}

CCopasiXMLParser::TextElement::~TextElement()
{}

void CCopasiXMLParser::TextElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{
  mCurrentElement++; /* We should always be on the next element */

  switch (mCurrentElement)
    {
    case Text:
      if (strcmp(pszName, "Text")) fatalError();
      break;

    default:
      fatalError();
      break;
    }

  return;
}

void CCopasiXMLParser::TextElement::end(const XML_Char *pszName)
{
  mStack.pop();

  switch (mCurrentElement)
    {
    case Text:
      if (strcmp(pszName, "Text")) fatalError();
      mCommon.pFunction->setDescription(mCommon.CharacterData);
      mCurrentElement = -1;

      /* Tell the parent element we are done. */
      mStack.top()->end(pszName);
      break;

    default:
      fatalError();
      break;
    }
}

CCopasiXMLParser::ListOfParameterDescriptionsElement::ListOfParameterDescriptionsElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common)
{}

CCopasiXMLParser::ListOfParameterDescriptionsElement::~ListOfParameterDescriptionsElement()
{
  pdelete(mpCurrentHandler);
}

void CCopasiXMLParser::ListOfParameterDescriptionsElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{
  mCurrentElement++; /* We should always be on the next element */

  switch (mCurrentElement)
    {
    case ListOfParameterDescriptions:
      if (strcmp(pszName, "ListOfParameterDescriptions")) fatalError();
      break;

    case ParameterDescription:
      if (strcmp(pszName, "ParameterDescription")) fatalError();

      /* If we do not have a ParameterDescription element handler we create one. */
      if (!mpCurrentHandler)
        mpCurrentHandler = new ParameterDescriptionElement(mCommon);

      /* Push the ParameterDescription element handler on the stack and call it. */
      mStack.push(mpCurrentHandler);
      mpCurrentHandler->start(pszName, papszAttrs);
      break;

    default:
      fatalError();
      break;
    }

  mCurrentElement++;
  return;
}

void CCopasiXMLParser::ListOfParameterDescriptionsElement::end(const XML_Char *pszName)
{
  mStack.pop();

  switch (mCurrentElement)
    {
    case ListOfParameterDescriptions:
      if (strcmp(pszName, "ListOfParameterDescriptions")) fatalError();

      /* Tell the parent element we are done. */
      mStack.top()->end(pszName);
      break;

    case ParameterDescription:
      if (strcmp(pszName, "ParameterDescription")) fatalError();
      break;

    default:
      fatalError();
      break;
    }

  mCurrentElement--;

  return;
}

CCopasiXMLParser::ParameterDescriptionElement::ParameterDescriptionElement(SCopasiXMLParserCommon & common) :
    CXMLElementHandler(common.ElementHandlerStack),
    mCommon(common)
{}

CCopasiXMLParser::ParameterDescriptionElement::~ParameterDescriptionElement()
{
  pdelete(mpCurrentHandler);
}

void CCopasiXMLParser::ParameterDescriptionElement::start(const XML_Char *pszName,
    const XML_Char **papszAttrs)
{
  mCurrentElement++; /* We should always be on the next element */

  switch (mCurrentElement)
    {
    case ParameterDescription:
      if (strcmp(pszName, "ParameterDescription")) fatalError();
      break;

    default:
      fatalError();
      break;
    }

  mCurrentElement++;
  return;
}

void CCopasiXMLParser::ParameterDescriptionElement::end(const XML_Char *pszName)
{
  mStack.pop();

  switch (--mCurrentElement)
    {
    case ParameterDescription:
      if (strcmp(pszName, "ParameterDescription")) fatalError();
      mCurrentElement = -1;
      /* Tell the parent element we are done. */
      mStack.top()->end(pszName);
      break;

    default:
      fatalError();
      break;
    }

  return;
}

CCopasiXMLParser::CCopasiXMLParser() :
    CExpat(),
    mCommon()
{
  create();

  mCommon.ElementHandlerStack.push(new COPASIElement(mCommon));
  enableElementHandler(true);
  enableCharacterDataHandler(true);
}

CCopasiXMLParser::~CCopasiXMLParser() {}

void CCopasiXMLParser::onStartElement(const XML_Char *pszName,
                                      const XML_Char **papszAttrs)
{
  assert(mCommon.ElementHandlerStack.size() != 0);
  mCommon.ElementHandlerStack.top()->start(pszName, papszAttrs);
}

void CCopasiXMLParser::onEndElement(const XML_Char *pszName)
{
  assert(mCommon.ElementHandlerStack.size() != 0);
  mCommon.ElementHandlerStack.top()->end(pszName);
}

void CCopasiXMLParser::onCharacterData(const XML_Char *pszData,
                                       int nLength)
{
  mCommon.CharacterData.assign(pszData, nLength);
}
