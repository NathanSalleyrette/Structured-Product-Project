import React, { useEffect, useState } from "react";
import Col from "react-bootstrap/Col";
import { Button, ToggleButtonGroup, ToggleButton, Form} from "react-bootstrap";

const ChartForm = (props) => {
  const [interval, setInterval] = useState("1d");
  //const [range, setRange] = useState("10y");
  const [startDate, setStartDate] = useState("2014-01-01");

  const [incorrectDate, setIncorrectDate] = useState(false);
  const todayDate = new Date().toISOString().slice(0, 10);
  const handleChangeInterval = (val) => setInterval(val);
  //const handleChangeRange = (val) => setRange(val);
  const handleChangeDate = (val) => {
    setStartDate(val.target.value);
  }

  useEffect(() => {
    console.log(startDate)
    if (startDate.trim() === "" || startDate > todayDate) setIncorrectDate(true);
    else setIncorrectDate(false);
  }, [startDate, todayDate])


  return (
    <>
      <Col>
      <h6>Frequency</h6>
        <ToggleButtonGroup
          type="radio"
          name="optionsInterval"
          defaultValue="1d"
          onChange={handleChangeInterval}
        >

          <ToggleButton
            id="tbg-radio-1-interval"
            value="1d"
            disabled={props.buttonDisabled}
          >
            One day
          </ToggleButton>
          <ToggleButton
            id="tbg-radio-2-interval"
            value="1wk"
            disabled={props.buttonDisabled}
            //disabled={true}
          >
            One week
          </ToggleButton>
          <ToggleButton
            id="tbg-radio-3-interval"
            value="1mo"
            disabled={props.buttonDisabled}
          >
            One month
          </ToggleButton>
        </ToggleButtonGroup>
      </Col>

      {/* <Col>
        <ToggleButtonGroup
          type="radio"
          name="optionsRange"
          defaultValue="10y"
          onChange={handleChangeRange}
        >
          <ToggleButton
            id="tbg-radio-1-range"
            value="10y"
            disabled={props.buttonDisabled}
          >
            10 years
          </ToggleButton>
          <ToggleButton
            id="tbg-radio-2-range"
            value="1y"
            disabled={props.buttonDisabled}
          >
            1 year
          </ToggleButton>
          <ToggleButton
            id="tbg-radio-3-range"
            value="1mo"
            disabled={props.buttonDisabled}
          >
            1 month
          </ToggleButton>
        </ToggleButtonGroup>
      </Col> */}
      <Col>
        <Form.Group controlId="picker">
          <Form.Label>Start Date</Form.Label>
          <Form.Control type="date" name="picker" value={startDate} min="2014-01-01" max={new Date().toISOString().slice(0, 10)} onChange={handleChangeDate} placeholder="Beginning Date" />
        </Form.Group>
      </Col>

      <Col>
        <Button
          type="submit"
          onClick={() => {
            props.onConnect(interval, startDate);
            //console.log(startDate);
          }}
          className="btn btn-primary btn-block"
          disabled={props.buttonDisabled || incorrectDate}
        >
          Validate
        </Button>
      </Col>
    </>
  );
};

export default ChartForm;
