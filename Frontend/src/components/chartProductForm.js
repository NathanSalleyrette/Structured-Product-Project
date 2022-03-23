import React, { useEffect, useState } from "react";
import Col from "react-bootstrap/Col";
import { Row , Card, Button, ToggleButtonGroup, ToggleButton, Form } from "react-bootstrap";

const BACKEND = "http://localhost:5000/api";
//const BACKEND = "https://peps-group1.herokuapp.com/api";

const ChartForm = (props) => {
  const [numberOfRebalancing, setNumberOfRebalancing] = useState("17");
  //const [range, setRange] = useState("10y");
  const [startDate, setStartDate] = useState("2018-01-01");
  const [deltaDate, setDeltaDate] = useState("2018-01-01");
  const [limitDate, setLimitDate] = useState();
  const [alert, setAlert] = useState("");
  const [alertMC, setAlertMC] = useState("");
  const [alertDelta, setAlertDelta] = useState("");
  const [nbMCSamples, setNbMCSamples] = useState(1000);
  const [simu, setSimu] = useState(false);
  const [incorrectDate, setIncorrectDate] = useState(false);
  const [incorrectDeltaDate, setIncorrectDeltaDate] = useState(false);
  const todayDate = new Date().toISOString().slice(0, 10);
  const handleChangeNumberOfRebalancing = (val) => setNumberOfRebalancing(val);
  //const handleChangeRange = (val) => setRange(val);


  const handleChangeDate = (val) => {
    setAlert('');
    setStartDate(val.target.value);
    var day = new Date(val.target.value).getUTCDay();
    if([6,0].includes(day)){
      setAlert('Weekends not allowed');
      setIncorrectDate(true);
    }
    else setIncorrectDate(false);
  }


  const handleChangeDeltaDate = (val) => {
    setAlertDelta('');
    setDeltaDate(val.target.value);
    var day = new Date(val.target.value).getUTCDay();
    if([6,0].includes(day)){
      setAlertDelta('Weekends not allowed');
      setIncorrectDeltaDate(true);
    }
    else setIncorrectDeltaDate(false);
  }


  const handleSimuChange = (val) => {
    setSimu(val.target.checked);
  }

  useEffect(() => {
    handleLoad()
  },[])

  function handlenbMCSamplesChange(e) {
    if (typeof e.target.value != "string") return false // we only process strings!  
    else if (!isNaN(e.target.value) && !isNaN(parseFloat(e.target.value))) {
      setAlertMC("");
      setNbMCSamples(e.target.value);}
    if(parseInt(e.target.value)< 100 || parseInt(e.target.value) > 10000){
      setAlertMC('MC samples need to be beetween 100 and 10000');
      setIncorrectDate(true);
    } else {
      setIncorrectDate(false);
    }
  }

  async function handleLoad() {

    await fetch(`${BACKEND}/limitDate`)
    .then((res) => res.json())
    .then((body) => {
      setLimitDate( 
        body.limitDate
      );
      setStartDate( 
        body.limitDate
      );
      setDeltaDate( 
        body.limitDate
      );
    })
    .catch(function (error) {
      console.log("error", error);
    });
  }

  

  return (
    <>
      <Col>
        <h6>Rebalancing Frequency</h6>
        <ToggleButtonGroup
          type="radio"
          name="optionsNumberOfRebalancing"
          defaultValue="17"
          onChange={handleChangeNumberOfRebalancing}
        >
          <ToggleButton
            id="tbg-radio-1-interval"
            value="417"
            disabled={props.buttonDisabled}
          >
            One week
          </ToggleButton>
          <ToggleButton
            id="tbg-radio-2-interval"
            value="193"
            disabled={props.buttonDisabled}
          >
            Two weeks
          </ToggleButton>
          <ToggleButton
            id="tbg-radio-3-interval"
            value="97"
            disabled={props.buttonDisabled}
          >
            One month
          </ToggleButton>
          <ToggleButton
            id="tbg-radio-4-interval"
            value="33"
            disabled={props.buttonDisabled}
          //disabled={true}
          >
            Three months
          </ToggleButton>
          <ToggleButton
            id="tbg-radio-5-interval"
            value="17"
            disabled={props.buttonDisabled}
          >
            Six months
          </ToggleButton>
        </ToggleButtonGroup>
        <Form.Check type="switch" label="Entirely simulated spots" onClick={handleSimuChange}/>
      </Col>
      <Col>
        <Form.Group controlId="picker">
          <Form.Label>Use Historic Spots until</Form.Label>
          {/* <Form.Control type="date" name="picker" value={startDate} min="2014-01-01" max={new Date().toISOString().slice(0, 10)} onChange={handleChangeDate} placeholder="Beginning Date" /> */}
          <Form.Control disabled={simu} type="date" name="picker" value={startDate} min="2014-07-17" max={limitDate} onChange={handleChangeDate} />
         

          <Form.Label>Number of MC samples</Form.Label>
          <Form.Control type="number" value={nbMCSamples} onChange={handlenbMCSamplesChange} />

        </Form.Group>
      </Col>

      <Col>
        <Row><Button
          type="submit"
          onClick={() => {
            if(simu){
              props.simpleRates(numberOfRebalancing, "1999-01-01", nbMCSamples);
            } else {
              props.simpleRates(numberOfRebalancing, startDate, nbMCSamples);
            }
            
          }}
          className="btn btn-primary btn-block"
          disabled={props.buttonDisabled || incorrectDate}
        >
          Launch Hedging Strategy
        </Button></Row>
        
        <Form.Label style={{color:"red"}}>{alert}</Form.Label>
        <Form.Label style={{color:"red"}}>{alertMC}</Form.Label>
      </Col>
      <Col>
        <Card
          bg="Primary"
          className="mb-2"
        >
          <Card.Header>P&L</Card.Header>
          <Card.Body>
            <Card.Text>
              {props.pAndL}%
            </Card.Text>
          </Card.Body>
        </Card>
      </Col>
      <Col>
      <Row>
      <Button
          type="submit"
          onClick={() => {
            props.getDeltas(deltaDate, nbMCSamples);
          }}
          className="btn btn-primary btn-block"
          disabled={props.buttonDisabled || incorrectDeltaDate}
        >
          Get Deltas
        </Button>
      </Row>
      <Row>
      <Form.Control type="date" name="picker" value={deltaDate} min="2014-07-17" max={limitDate} onChange={handleChangeDeltaDate} />
        </Row>
        <Row>
                <Form.Label style={{color:"red"}}>{alertDelta}</Form.Label>
        </Row>
      </Col>
    </>
  );
};

export default ChartForm;
